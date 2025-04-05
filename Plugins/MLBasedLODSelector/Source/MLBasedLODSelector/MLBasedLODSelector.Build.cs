// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class MLBasedLODSelector : ModuleRules
{
	public MLBasedLODSelector(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // ONNX Runtime ��� ���� (������Ʈ ������ �°� ����)
        string ONNXRuntimePath = "D:/unreal_project/onnxruntime-win-x64-1.20.1";

        // Include ��� �߰�
        PublicIncludePaths.Add(Path.Combine(ONNXRuntimePath, "include"));

        // ���̺귯�� ��� �߰�
        PublicAdditionalLibraries.Add(Path.Combine(ONNXRuntimePath, "lib", "onnxruntime.lib"));

        // ���� ������ DLL�� ã�� �� �ֵ��� RuntimeDependencies ���� (��δ� ���� DLL ��ġ�� ���� ����)
        RuntimeDependencies.Add(Path.Combine(ONNXRuntimePath, "lib", "onnxruntime.dll"));


        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"Slate",
				"SlateCore",
				"RenderCore",
				"Renderer",
				"RHI",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
