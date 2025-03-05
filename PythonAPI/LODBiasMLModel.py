import os
import math
import random
import pandas as pd
import numpy as np
from PIL import Image

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader

# ------------------------------
# PSNR 계산 함수
# ------------------------------
def compute_psnr(img1: np.ndarray, img2: np.ndarray, max_val=255.0):
    mse = np.mean((img1.astype(np.float32) - img2.astype(np.float32)) ** 2)
    if mse < 1e-10:
        return 100.0
    return 10.0 * math.log10((max_val ** 2) / mse)

# ------------------------------
# Dataset
# ------------------------------
class LODBiasChunkDataset(Dataset):
    """
    CSV가 다음처럼 5행씩 반복된다고 가정:
      (feat1..feat5, fps_LOD1, LOD=1)
      (feat1..feat5, fps_LOD2, LOD=2)
      (feat1..feat5, fps_LOD3, LOD=3)
      (feat1..feat5, fps_LOD4, LOD=4)
      (feat1..feat5, fps_LOD5, LOD=5)
    """

    def __init__(self,
                 csv_path: str,
                 lod_root_dirs: dict,   
                 gt_root_dir: str,   
                 alpha=0.5,
                 beta=0.5,
                 basis_lod=3,
                 use_lod5_as_gt=False):

        super().__init__()
        self.df = pd.read_csv(csv_path).reset_index(drop=True)
        self.alpha = alpha
        self.beta = beta
        self.basis_lod = basis_lod
        self.lod_root_dirs = lod_root_dirs
        self.gt_root_dir = gt_root_dir
        self.use_lod5_as_gt = use_lod5_as_gt

        self.num_scenes = len(self.df) // 5

    def __len__(self):
        return self.num_scenes

    def _load_image_np(self, path: str):
        img = Image.open(path).convert("RGB")
        return np.array(img)

    def __getitem__(self, idx):
        # CSV에서 5행 chunk를 뽑음
        start_row = idx * 5
        end_row = start_row + 5
        chunk = self.df.iloc[start_row:end_row] 

        # feature1..5는 모두 동일
        row0 = chunk.iloc[0]
        x_features = np.array(row0[0:5], dtype=np.float32)

        lod_list = chunk.iloc[:, 6].values 
        fps_list = chunk.iloc[:, 5].values 

        scene_filename = f"img_{idx:04d}.png"
        if not self.use_lod5_as_gt:
            gt_path = os.path.join(self.gt_root_dir, scene_filename)
            gt_img_np = self._load_image_np(gt_path)
        else:
            gt_img_np = None 

        # 각 LOD별 PSNR, FPS를 이용해 score 계산
        psnr_list = []
        for i in range(5):
            lod_val = lod_list[i]  
            fps_val = float(fps_list[i])

            # LOD별 이미지 로딩
            lod_dir = self.lod_root_dirs[lod_val] 
            render_path = os.path.join(lod_dir, scene_filename)
            render_np = self._load_image_np(render_path)

            if self.use_lod5_as_gt and lod_val == 5:
                gt_img_np = render_np

            if gt_img_np is not None:
                psnr_ = compute_psnr(render_np, gt_img_np)
            else:
                psnr_ = 0.0 #예외처리

            psnr_list.append(psnr_)

        # psnr_list, fps_list 길이=5
        # 정규화 위해 각각의 최대값 구함
        max_fps = max(fps_list)
        max_psnr = max(psnr_list)

        best_score = -9999.0
        best_lod = None
        for i in range(5):
            # alpha * (fps_i / max_fps) + beta * (psnr_i / max_psnr)
            score_i = self.alpha*(fps_list[i]/(max_fps+1e-9)) + self.beta*(psnr_list[i]/(max_psnr+1e-9))
            if score_i > best_score:
                best_score = score_i
                best_lod = lod_list[i]

        # label = best_lod - basis_lod
        label = float(best_lod - self.basis_lod)

        x_tensor = torch.tensor(x_features, dtype=torch.float32)
        y_tensor = torch.tensor(label, dtype=torch.float32)
        return x_tensor, y_tensor

# ------------------------------
# 임시 모델
# ------------------------------
class LODBiasModel(nn.Module):
    def __init__(self, input_dim=5, hidden_dim=64):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(input_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, hidden_dim),
            nn.ReLU(),
            nn.Linear(hidden_dim, 1)  # output: bias
        )

    def forward(self, x):
        return self.net(x)

if __name__ == "__main__":
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    #임시 경로
    csv_path = "data.csv"
    
    lod_dirs = {
        1: "LOD1",
        2: "LOD2",
        3: "LOD3",
        4: "LOD4",
        5: "LOD5",
    }
    gt_dir = "GT"

    dataset = LODBiasChunkDataset(
        csv_path=csv_path,
        lod_root_dirs=lod_dirs,
        gt_root_dir=gt_dir,
        alpha=0.5,
        beta=0.5,
        basis_lod=3,
        use_lod5_as_gt=False 
    )

    print("Number of scenes:", len(dataset)) 

    # train/val split
    train_size = int(len(dataset)*0.8)
    val_size = len(dataset) - train_size
    train_dataset, val_dataset = torch.utils.data.random_split(dataset, [train_size, val_size])

    train_loader = DataLoader(train_dataset, batch_size=4, shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=4, shuffle=False)

    model = LODBiasModel(input_dim=5, hidden_dim=64)
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-3)

    model.to(device)

    num_epochs = 5
    for epoch in range(1, num_epochs+1):
        model.train()
        total_train_loss = 0.0
        for x_batch, y_batch in train_loader:
            x_batch = x_batch.to(device)
            y_batch = y_batch.to(device)

            pred = model(x_batch).squeeze()
            loss = criterion(pred, y_batch)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            total_train_loss += loss.item() * x_batch.size(0)
        avg_train_loss = total_train_loss / len(train_loader.dataset)

        # Validation
        model.eval()
        total_val_loss = 0.0
        with torch.no_grad():
            for x_batch, y_batch in val_loader:
                x_batch = x_batch.to(device)
                y_batch = y_batch.to(device)
                pred = model(x_batch).squeeze()
                loss = criterion(pred, y_batch)
                total_val_loss += loss.item() * x_batch.size(0)
        avg_val_loss = total_val_loss / len(val_loader.dataset)

        print(f"[Epoch {epoch}/{num_epochs}] Train Loss: {avg_train_loss:.4f} | Val Loss: {avg_val_loss:.4f}")

    # 임시 추론
    model.eval()
    sample_idx = 0  
    x_input, y_label = dataset[sample_idx]
    print("Scene #0 => True label(bias):", float(y_label))
    x_input = x_input.unsqueeze(0).to(device)  
    with torch.no_grad():
        pred_bias = model(x_input).item()
    print(f"Predicted bias: {pred_bias:.3f}")
    actual_lod = 3.0 + pred_bias
    final_lod = max(1.0, min(5.0, actual_lod))
    print(f" => LOD = {final_lod:.2f} (clamped 1~5)")
