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
    """
    path의 이미지를 Pillow로 열어서 NumPy (H,W,3)로 반환.
    """
    img = Image.open(path).convert("RGB")
    return np.array(img)

def compute_psnr_with_sort(
    csv_in: str,
    csv_out: str,
    lod_root_dirs: dict,
    gt_root_dir: str,
    use_lod1_as_gt: bool = False,
    scene_col: str = "sceneID",
    features: list = None,
    fps_col: str = "fps",
    lod_col: str = "LOD"
):
    """
    1) CSV를 읽고, [sceneID, feature1..5, LOD] 순으로 정렬
    2) 각 행(sceneID, LOD)에 해당하는 이미지를 로딩 → GT 이미지와 PSNR 계산
    3) 'PSNR' 컬럼으로 저장하고 최종 CSV 출력

    lod_root_dirs: {1: "LOD1/", 2: "LOD2/", ...}  # LOD 별 디렉토리
    gt_root_dir: "GT/"  # GT 이미지 폴더 (use_lod1_as_gt=False 시 사용)
    use_lod1_as_gt: True 시, LOD=1 이미지를 GT로 간주할 수도 있음(임의 로직 필요)

    features: ["feature1","feature2","feature3","feature4","feature5"]
      CSV에서 feature 컬럼 이름 리스트. 정렬 시 활용.
    """
    if features is None:
        # 디폴트 예시로 feature1..feature5
        features = ["feature1","feature2","feature3","feature4","feature5"]

    # 1) CSV 불러오기
    df = pd.read_csv(csv_in)

    # 2) 정렬
    sort_cols = [scene_col] + features + [lod_col]
    # 모든 열을 오름차순으로 정렬
    df = df.sort_values(by=sort_cols, ascending=True).reset_index(drop=True)

    # 3) PSNR 계산할 열 생성
    psnr_values = []

    for i in range(len(df)):
        row = df.iloc[i]
        scene_id = row[scene_col]
        lod_val = int(float(row[lod_col]))
        

        fname = f"{scene_id}.png" 
        lod_dir = lod_root_dirs[lod_val] 
        render_path = os.path.join(lod_dir, fname)
        render_np = load_image_as_numpy(render_path)

        # GT 이미지 로드 (use_lod1_as_gt=False 라면 GT 폴더에서 로드)
        if use_lod1_as_gt:
            # LOD=1 이미지가 GT라고 가정 → sceneID+LOD=1 파일을 로드
            # 별도 로직으로 "항상 lod=1 폴더에서 scene_id 이미지를 GT로 로드"
            # 아래는 가장 간단히: "LOD1" 폴더에서 scene_id 이미지가 GT라고 가정.
            gt_path = os.path.join(lod_root_dirs[1], fname)
            gt_np = load_image_as_numpy(gt_path)
        else:
            # GT 폴더에서 동일 파일명 로드
            gt_path = os.path.join(gt_root_dir, fname)
            gt_np = load_image_as_numpy(gt_path)

        # PSNR 계산
        psnr_ = compute_psnr(render_np, gt_np)
        psnr_values.append(psnr_)

    # df에 'PSNR' 열 추가
    df["PSNR"] = psnr_values

    # CSV 저장
    df.to_csv(csv_out, index=False)
    print(f"[Done] Sorted by {sort_cols}, computed PSNR, saved -> {csv_out}")

# -------------------------------------------
# 메인 실행 예시
# -------------------------------------------
if __name__ == "__main__":
    # 예시 파라미터
    csv_in  = "D:/unreal_project/SimpleShooter/Saved/MLLogs/ActorDataLog.csv"     # 원본 CSV
    csv_out = "D:/unreal_project/SimpleShooter/Saved/MLLogs/ActorDataLog2.csv"  # 정렬 + PSNR 추가된 최종 CSV

    lod_dirs = {
        1: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_1",
        2: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_2",
        3: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_3",
        4: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_4",
        5: "D:/unreal_project/SimpleShooter/Saved/MLScreenshots/LOD_5"
    }
    gt_dir = "GT"

    # 함수 실행
    compute_psnr_with_sort(
        csv_in=csv_in,
        csv_out=csv_out,
        lod_root_dirs=lod_dirs,
        gt_root_dir=gt_dir,
        use_lod1_as_gt=True,  # LOD1를 GT로 쓰려면 True
        scene_col="sceneID",   # CSV 열 이름
        features=["Distance","ScreenBound","NumTriangle","NumMatrial","MemoryUsage"],
        fps_col="fps",         # FPS 열 이름(필요시)
        lod_col="LodBiasPred"          # LOD 열 이름
    )
