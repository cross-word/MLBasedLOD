import unreal

TARGET_LOD_COUNT = 5

def generate_missing_lods_for_mesh(mesh_asset, target_lod_count):
    current_lod_count = mesh_asset.get_num_lods() 
    unreal.log("Mesh '{}' has {} LODs".format(mesh_asset.get_name(), current_lod_count))

    if current_lod_count >= target_lod_count:
        return

    for lod_index in range(current_lod_count, target_lod_count):
        unreal.log("Generating LOD {} for mesh '{}'".format(lod_index, mesh_asset.get_name()))
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

batch_process_static_meshes("/All/Game/AssetPacks")
