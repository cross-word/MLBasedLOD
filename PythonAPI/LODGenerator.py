import unreal

TARGET_LOD_COUNT = 5

def generate_missing_lods_for_mesh(mesh_asset, target_lod_count):
    # 현재 메시의 LOD 수를 확인 (StaticMesh 객체의 API에 따라 다를 수 있음)
    current_lod_count = mesh_asset.get_num_lods()  # 예시 함수, 실제 API는 다를 수 있음
    unreal.log("Mesh '{}' has {} LODs".format(mesh_asset.get_name(), current_lod_count))

    if current_lod_count >= target_lod_count:
        return

    # MeshReductionInterface를 통해 자동 LOD 생성을 시도합니다.
    # (아래 코드는 개념적인 예시이며, 실제 API 호출은 프로젝트에 따라 구현해야 합니다.)
    for lod_index in range(current_lod_count, target_lod_count):
        unreal.log("Generating LOD {} for mesh '{}'".format(lod_index, mesh_asset.get_name()))
        # 예시: mesh_asset.generate_lod(lod_index) 또는 유사한 함수를 호출
        # 만약 직접 구현한다면, mesh reduction 설정을 구성한 후 해당 LOD를 생성하도록 호출합니다.
        # 예: unreal.StaticMeshEditorSubsystem.generate_lod(mesh_asset, lod_index, reduction_settings)
        # 아래는 개념 증명을 위한 로그 출력입니다.
    
    # 변경된 메시를 저장합니다.
    asset_path = mesh_asset.get_path_name()
    if unreal.EditorAssetLibrary.save_asset(asset_path):
        unreal.log("Saved mesh '{}' with new LODs".format(mesh_asset.get_name()))
    else:
        unreal.log_warning("Failed to save mesh '{}'".format(mesh_asset.get_name()))

def batch_process_static_meshes(folder_path):
    asset_paths = unreal.EditorAssetLibrary.list_assets(folder_path, recursive=True, include_folder=True)
    for asset_path in asset_paths:
        asset = unreal.EditorAssetLibrary.load_asset(asset_path)
        if isinstance(asset, unreal.StaticMesh):
            generate_missing_lods_for_mesh(asset, TARGET_LOD_COUNT)

# 실행할 폴더 경로 설정 (예: '/Game/Meshes')
batch_process_static_meshes("/All/Game/AssetPacks")
