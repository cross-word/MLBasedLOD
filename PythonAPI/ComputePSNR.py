import os
import math
import numpy as np
import pandas as pd
from PIL import Image

def compute_psnr(img1: np.ndarray, img2: np.ndarray, max_val=255.0):
    """
    두 이미지를 NumPy (H, W, 3) 형태로 받아 PSNR을 계산.
    """
    mse = np.mean((img1.astype(np.float32) - img2.astype(np.float32)) ** 2)
    if mse < 1e-10:
        return 100.0
    return 10.0 * math.log10((max_val ** 2) / mse)

def load_image_as_numpy(path: str):
    img = Image.open(path).convert("RGB")
    return np.array(img)

def precompute_psnr_with_sort(
    csv_in: str,
    csv_out: str,
    lod_root_dirs: dict,
    gt_root_dir: str,
    use_lod5_as_gt: bool = False,
    scene_col: str = "sceneID",
    features: list = None,
    fps_col: str = "fps",
    lod_col: str = "LOD"
):
    if features is None:
        features = ["feature1","feature2","feature3","feature4","feature5"]

    df = pd.read_csv(csv_in)

    # 정렬
    sort_cols = [scene_col] + features + [lod_col]
    df = df.sort_values(by=sort_cols, ascending=True).reset_index(drop=True)

    # PSNR 열 생성
    psnr_values = []

    for i in range(len(df)):
        row = df.iloc[i]
        scene_id = row[scene_col]
        lod_val = int(row[lod_col])

        fname = f"{scene_id}.png" 
        lod_dir = lod_root_dirs[lod_val] 
        render_path = os.path.join(lod_dir, fname)
        render_np = load_image_as_numpy(render_path)

        if use_lod5_as_gt:
            gt_path = os.path.join(lod_root_dirs[5], fname)
            gt_np = load_image_as_numpy(gt_path)
        else:
            gt_path = os.path.join(gt_root_dir, fname)
            gt_np = load_image_as_numpy(gt_path)

        psnr_ = compute_psnr(render_np, gt_np)
        psnr_values.append(psnr_)

    # df에 'PSNR' 열 추가
    df["PSNR"] = psnr_values

    # CSV 저장
    df.to_csv(csv_out, index=False)
    print(f"[Done] Sorted by {sort_cols}, computed PSNR, saved -> {csv_out}")

if __name__ == "__main__":
    # 임시 경로
    csv_in  = "D:/unreal_project/LODPlugin/Saved/MLLogs/ActorDataLog.csv"     # input
    csv_out = "D:/unreal_project/LODPlugin/Saved/MLLogs/ActorDataLog2.csv"  # output

    lod_dirs = {
        1: "D:/unreal_project/LODPlugin/Saved/MLScreenshots/LOD_1",
        2: "D:/unreal_project/LODPlugin/Saved/MLScreenshots/LOD_2",
        3: "D:/unreal_project/LODPlugin/Saved/MLScreenshots/LOD_3",
        4: "D:/unreal_project/LODPlugin/Saved/MLScreenshots/LOD_4",
        5: "D:/unreal_project/LODPlugin/Saved/MLScreenshots/LOD_5"
    }
    gt_dir = "GT"

    precompute_psnr_with_sort(
        csv_in=csv_in,
        csv_out=csv_out,
        lod_root_dirs=lod_dirs,
        gt_root_dir=gt_dir,
        use_lod5_as_gt=True,
        scene_col="sceneID", 
        features=["Distance","ScreenBound","NumTriangle","NumMatrial","MemoryUsage"],
        fps_col="fps",         
        lod_col="LodBiasPred"         
    )
