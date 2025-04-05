import os
import math
import numpy as np
import pandas as pd
from PIL import Image
import torch

def compute_psnr(img1: np.ndarray, img2: np.ndarray, max_val=255.0):
    # 두 이미지를 (H, W, 3) 배열로 받아 PSNR을 계산
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    a = torch.tensor(img1, dtype=torch.float32, device=device)
    b = torch.tensor(img2, dtype=torch.float32, device=device)
    
    mse = torch.mean((a - b) ** 2)
    mse_val = mse.item()
    if mse_val < 1e-10:
        return 100.0
    return 10.0 * math.log10((max_val ** 2) / mse_val)

def load_image_as_numpy(path: str):
    # path의 이미지를 열어서 (H,W,3)로 반환.
    img = Image.open(path).convert("RGB")
    return np.array(img)

def compute_psnr_with_sort(
    csv_in: str,
    csv_out: str,
    lod_root_dirs: dict,
    gt_root_dir: str,
    scene_col: str = "sceneID",
    features: list = None,
    lod_col: str = "LOD"
):
    """
    1) CSV를 읽고, [sceneID, feature1..5, LOD] 순으로 정렬
    2) 각 행(sceneID, LOD)에 해당하는 이미지를 로딩 하여 PSNR 계산
    3) 'PSNR' 컬럼을 추가하고 추가한 CSV 저장

    lod_root_dirs: LOD 별 디렉토리
    gt_root_dir: GT 이미지 폴더

    features: ["feature1","feature2","feature3","feature4","feature5"]
    """

    # csv를 불러와 scene id를 기준으로 정렬
    df = pd.read_csv(csv_in)
    sort_cols = [scene_col] + features + [lod_col]
    df = df.sort_values(by=sort_cols, ascending=True).reset_index(drop=True)

    # psnr 계산시 이미 계산한 이미지면 계산하지않고 불러움
    psnr_values = []
    psnr_save = {}

    for i in range(len(df)):
        row = df.iloc[i]
        scene_id = row[scene_col]
        lod_val = int(float(row[lod_col]))
        key = (scene_id, lod_val)
        
        if key in psnr_save:
            psnr_ = psnr_save[key]
        else:
            fname = f"{scene_id}.png" 
            lod_dir = lod_root_dirs[lod_val] 
            render_path = os.path.join(lod_dir, fname)
            render_np = load_image_as_numpy(render_path)

            # GT 폴더에서 동일 파일명 로드
            gt_path = os.path.join(gt_root_dir, fname)
            gt_np = load_image_as_numpy(gt_path)

            psnr_ = compute_psnr(render_np, gt_np)

        psnr_values.append(psnr_)

    # PSNR 열에 값 추가 후 저장
    df["PSNR"] = psnr_values
    df.to_csv(csv_out, index=False)
    print(f"[Done] Sorted by {sort_cols}, computed PSNR, saved -> {csv_out}")

# -------------------------------------------
# 메인 실행 예시
# -------------------------------------------
if __name__ == "__main__":
    # 예시 파라미터
    csv_in  = "D:/unreal_project/SimpleShooter/Saved/MLLogs/ActorDataLog.csv"     # 원본 CSV
    csv_out = "D:/unreal_project/SimpleShooter/Saved/MLLogs/ActorDataLog2.csv"  # 저장될 CSV파일 이름

    lod_dirs = {
        1: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_1",
        2: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_2",
        3: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_3",
        4: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_4",
        5: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_5"
    }
    gt_dir = "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_0"

    compute_psnr_with_sort(
        csv_in=csv_in,
        csv_out=csv_out,
        lod_root_dirs=lod_dirs,
        gt_root_dir=gt_dir,
        scene_col="sceneID",
        features=["Distance","ScreenBound","NumTriangle","NumMatrial","MemoryUsage"],
        lod_col="LodBiasPred" 
    )