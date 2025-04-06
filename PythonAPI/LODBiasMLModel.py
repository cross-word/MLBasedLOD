import os
import pandas as pd
import numpy as np

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
import argparse

parser = argparse.ArgumentParser(description='MLBasedLODBias')
parser.add_argument('--lr', default=0.0001, type=float, help='learning rate')
parser.add_argument('--batch_size', default=128, type=int,
                    help='batch size')
parser.add_argument('--optimizer', default="adam", type=str,
                    help='specify optimizer')
parser.add_argument('--epoch', default=1000, type=int,
                    help='learning epoch')
parser.add_argument('--PSNR_ratio', default=0.5, type=float,
                    help='PSNR:fps ration. should in range [0,1]')
parser.add_argument('--data_path', type=str,
                    help='.csv data path')
parser.add_argument('--csv_name', type=str,
                    help='.csv file name')

args = parser.parse_args()

class LODBiasChunkDataset(Dataset):
    """
    CSV가 다음처럼 5행씩 반복된다고 가정:
      (ScencID, feat1..feat5, fps_LOD0, LOD=1, PSNR)
      (ScencID, feat1..feat5, fps_LOD1, LOD=1, PSNR)
      (ScencID, feat1..feat5, fps_LOD2, LOD=2, PSNR)
      (ScencID, feat1..feat5, fps_LOD3, LOD=3, PSNR)
      (ScencID, feat1..feat5, fps_LOD4, LOD=5, PSNR)
    이 5행을 하나의 '씬'/'상황'으로 묶어서 처리
    """

    def __init__(self,
                 csv_path: str,
                 lod_root_dirs: dict, 
                 gt_root_dir: str,
                 alpha=args.PSNR_ratio,
                 beta=1-args.PSNR_ratio,):
        
        super().__init__()
        self.df = pd.read_csv(csv_path).reset_index(drop=True)
        self.alpha = alpha
        self.beta = beta
        self.lod_root_dirs = lod_root_dirs
        self.gt_root_dir = gt_root_dir

        # 씬 개수
        self.num_scenes = len(self.df) // 5

    def __len__(self):
        return self.num_scenes

    def __getitem__(self, idx):
        """
        idx번째 씬에 해당하는 CSV row chunk(5행)를 가져옴
        """
        # CSV에서 5행 chunk를 뽑음
        start_row = idx * 5
        end_row = start_row + 5
        chunk = self.df.iloc[start_row:end_row]

        # feature1..5는 모두 동일하다고 가정
        row0 = chunk.iloc[0]
        x_features = np.array(row0[1:6], dtype=np.float32)

        # 5개 LOD 각각 fps, LOD가 기록됨
        # 행마다 형태는 [scene_id, feat1, feat2, feat3, feat4, feat5, fps_LOD?, LOD?, PSNR?]
        lod_list = chunk.iloc[:, 7].values 
        fps_list = chunk.iloc[:, 6].values 
        psnr_list = chunk.iloc[:, 8].values

        best_score = -9999.0
        best_lod = None
        for i in range(5):
            # alpha * 1/ms + beta * psnr
            score_i = self.alpha*(1/fps_list[i]) + self.beta*(psnr_list[i])
            if score_i > best_score:
                best_score = score_i
                best_lod = lod_list[i]

        label = float(best_lod) 
        x_tensor = torch.tensor(x_features, dtype=torch.float32)
        y_tensor = torch.tensor(label, dtype=torch.float32)
        return x_tensor, y_tensor

# ------------------------------
# 모델 학습
# ------------------------------
'''
input
1 - Distance
2 - Screen bound
3 - Num triangle
4 - Memory use
5 - Num matrial

out - LODBias range in [0,1]
'''
class LODBiasModel(nn.Module):
    def __init__(self):
        super().__init__()

        input_dim_all = 5
        input_dim_DistBoundMatMem = 4
        input_dim_DistBound = 2
        input_dim_MatMem = 2
        dropout_rate = 0.2

        # Layer 1: all feature
        self.net_all = nn.Sequential(
            nn.Linear(input_dim_all, 256),
            nn.BatchNorm1d(256),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(256, 128),
            nn.BatchNorm1d(128),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(128, 5)
        )

        # Layer 2: except num_tri
        self.net_DistBoundMatMem = nn.Sequential(
            nn.Linear(input_dim_DistBoundMatMem, 128),
            nn.BatchNorm1d(128),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(128, 64),
            nn.BatchNorm1d(64),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(64, 4)
        )

        # Layer 3: Dist and Bound
        self.net_DistBound = nn.Sequential(
            nn.Linear(input_dim_DistBound, 64),
            nn.BatchNorm1d(64),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(64, 32),
            nn.BatchNorm1d(32),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(32, 2)
        )

        # Layer 4: Mem and Matiral
        self.net_MatMem = nn.Sequential(
            nn.Linear(input_dim_MatMem, 64),
            nn.BatchNorm1d(64),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(64, 32),
            nn.BatchNorm1d(32),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(32, 2)
        )
        #Layer 5: Output Layer
        self.net_out = nn.Sequential(
            nn.Linear(13, 256),
            nn.BatchNorm1d(256),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(256, 256),
            nn.BatchNorm1d(256),
            nn.ReLU(),
            nn.Dropout(dropout_rate),
            nn.Linear(256, 1),
            nn.Sigmoid()
        )

        self._init_weights()

    def _init_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Linear):
                nn.init.xavier_uniform_(m.weight)
                if m.bias is not None:
                    nn.init.constant_(m.bias, 0)
            elif isinstance(m, (nn.BatchNorm1d, nn.LayerNorm)):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, 0)

    def forward(self, x):
        x_all = x
        x_DB = x[:, :2]
        x_MM = x[:, 3:]
        x_DBMM = torch.cat([x_DB, x_MM], dim=1)

        x_all_ = self.net_all(x_all)
        x_all_ = x_all_ + x_all
        x_DBMM_ = self.net_DistBoundMatMem(x_DBMM)
        x_DBMM_ = x_DBMM_ + x_DBMM
        x_DB_ = self.net_DistBound(x_DB)
        x_DB_ = x_DB_ + x_DB
        x_MM_ = self.net_MatMem(x_MM)
        x_MM_ = x_MM_ + x_MM

        x_out = torch.cat([x_all_, x_DBMM_, x_DB_, x_MM_], dim=1)
        x_out = self.net_out(x_out)
        x_out = 5 * x_out

        return x_out




if __name__ == "__main__":
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    # feature1 feature2 feature3 feature4 feature5 1/fps LOD PSNR
    # 같은 feature에 LOD다른버전 5개 행에 연속으로 존재
    csv_path = os.path.join(args.data_path, args.csv_name)

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
        alpha=args.PSNR_ratio,
        beta=1-args.PSNR_ratio
    )

    print("Number of scenes:", len(dataset))

    # train/val split
    train_size = int(len(dataset)*0.7)
    val_size   = int(len(dataset)*0.2)
    test_size = len(dataset) - train_size - val_size
    train_dataset, val_dataset, test_dataset = torch.utils.data.random_split(dataset, [train_size, val_size, test_size])

    train_loader = DataLoader(train_dataset, batch_size=args.batch_size, shuffle=True)
    val_loader = DataLoader(val_dataset, batch_size=args.batch_size, shuffle=False)
    test_loader  = DataLoader(test_dataset, batch_size=1, shuffle=False)

    model = LODBiasModel()
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=args.lr, weight_decay=1e-4)
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=50, gamma=0.95)

    model.to(device)

    num_epochs = args.epoch
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
        scheduler.step()

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

    model.eval()
    total_test_loss = 0.0
    with torch.no_grad():
        for x_batch, y_batch in test_loader:
            x_batch = x_batch.to(device)
            y_batch = y_batch.to(device)
            pred = model(x_batch).squeeze()
            loss = criterion(pred, y_batch)
            total_test_loss += loss.item() * x_batch.size(0)
    avg_test_loss = total_test_loss / test_size

    print(f"Test Loss: {avg_test_loss:.4f}")

    with torch.no_grad():
        for i, (x, y) in enumerate(test_loader):
            x = x.to(device)
            pred = model(x).squeeze()
            print("예측값:", pred.cpu().numpy(), "실제 LOD값:", y.numpy())
            if i == 0:
                break
            
    #torch.save(model.state_dict(), os.path.join(args.data_path, "model.pth"))
    #traced_script_module = torch.jit.trace(model, torch.rand(1, 5, device=device))
    #traced_script_module.save(os.path.join(args.data_path, "model_script.pt"))
    torch.onnx.export(model, torch.rand(1, 5, device=device), "model.onnx", input_names=['input'], output_names=['output']) # UE inference를 위해 onnx로 저장