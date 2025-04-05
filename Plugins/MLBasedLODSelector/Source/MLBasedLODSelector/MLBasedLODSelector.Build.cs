// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class MLBasedLODSelector : ModuleRules
{
	public MLBasedLODSelector(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // ONNX Runtime 경로 설정 (프로젝트 구조에 맞게 수정)
        string ONNXRuntimePath = "D:/unreal_project/onnxruntime-win-x64-1.20.1";

        // Include 경로 추가
        PublicIncludePaths.Add(Path.Combine(ONNXRuntimePath, "include"));

        // 라이브러리 경로 추가
        PublicAdditionalLibraries.Add(Path.Combine(ONNXRuntimePath, "lib", "onnxruntime.lib"));

        // 실행 파일이 DLL을 찾을 수 있도록 RuntimeDependencies 설정 (경로는 실제 DLL 위치에 따라 수정)
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
