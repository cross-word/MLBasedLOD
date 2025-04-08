// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MLBasedLODSelector/Public/NaniteMLManager.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNaniteMLManager() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
MLBASEDLODSELECTOR_API UClass* Z_Construct_UClass_UNaniteMLManager();
MLBASEDLODSELECTOR_API UClass* Z_Construct_UClass_UNaniteMLManager_NoRegister();
UPackage* Z_Construct_UPackage__Script_MLBasedLODSelector();
// End Cross Module References

// Begin Class UNaniteMLManager
void UNaniteMLManager::StaticRegisterNativesUNaniteMLManager()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UNaniteMLManager);
UClass* Z_Construct_UClass_UNaniteMLManager_NoRegister()
{
	return UNaniteMLManager::StaticClass();
}
struct Z_Construct_UClass_UNaniteMLManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/*\n  onnxruntime\xec\x9c\xbc\xeb\xa1\x9c ML\xeb\xaa\xa8\xeb\x8d\xb8 \xeb\xa1\x9c\xeb\x93\x9c/\xec\x96\xb8\xeb\xa1\x9c\xeb\x93\x9c\n  N\xed\x94\x84\xeb\xa0\x88\xec\x9e\x84(\xec\xa1\xb0\xec\xa0\x95\xec\xa4\x91)\xeb\xa7\x88\xeb\x8b\xa4 \xeb\xaa\xa8\xeb\x8d\xb8\xec\x97\x90 inference\xed\x95\x98\xec\x97\xac \xec\x95\xa1\xed\x84\xb0\xeb\xa7\x88\xeb\x8b\xa4 LOD\xeb\xa5\xbc \xea\xb3\x84\xec\x82\xb0 \xed\x9b\x84 LOD\xea\xb0\x95\xec\xa0\x9c\n */" },
#endif
		{ "IncludePath", "NaniteMLManager.h" },
		{ "ModuleRelativePath", "Public/NaniteMLManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "onnxruntime\xec\x9c\xbc\xeb\xa1\x9c ML\xeb\xaa\xa8\xeb\x8d\xb8 \xeb\xa1\x9c\xeb\x93\x9c/\xec\x96\xb8\xeb\xa1\x9c\xeb\x93\x9c\nN\xed\x94\x84\xeb\xa0\x88\xec\x9e\x84(\xec\xa1\xb0\xec\xa0\x95\xec\xa4\x91)\xeb\xa7\x88\xeb\x8b\xa4 \xeb\xaa\xa8\xeb\x8d\xb8\xec\x97\x90 inference\xed\x95\x98\xec\x97\xac \xec\x95\xa1\xed\x84\xb0\xeb\xa7\x88\xeb\x8b\xa4 LOD\xeb\xa5\xbc \xea\xb3\x84\xec\x82\xb0 \xed\x9b\x84 LOD\xea\xb0\x95\xec\xa0\x9c" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNaniteMLManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UNaniteMLManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_MLBasedLODSelector,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UNaniteMLManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UNaniteMLManager_Statics::ClassParams = {
	&UNaniteMLManager::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UNaniteMLManager_Statics::Class_MetaDataParams), Z_Construct_UClass_UNaniteMLManager_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UNaniteMLManager()
{
	if (!Z_Registration_Info_UClass_UNaniteMLManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNaniteMLManager.OuterSingleton, Z_Construct_UClass_UNaniteMLManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UNaniteMLManager.OuterSingleton;
}
template<> MLBASEDLODSELECTOR_API UClass* StaticClass<UNaniteMLManager>()
{
	return UNaniteMLManager::StaticClass();
}
UNaniteMLManager::UNaniteMLManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UNaniteMLManager);
UNaniteMLManager::~UNaniteMLManager() {}
// End Class UNaniteMLManager

// Begin Registration
struct Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UNaniteMLManager, UNaniteMLManager::StaticClass, TEXT("UNaniteMLManager"), &Z_Registration_Info_UClass_UNaniteMLManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNaniteMLManager), 565452414U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_1565730631(TEXT("/Script/MLBasedLODSelector"),
	Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
