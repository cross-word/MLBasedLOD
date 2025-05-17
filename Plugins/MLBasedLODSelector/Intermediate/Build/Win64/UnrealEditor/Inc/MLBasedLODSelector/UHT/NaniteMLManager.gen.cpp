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
		{ "IncludePath", "NaniteMLManager.h" },
		{ "ModuleRelativePath", "Public/NaniteMLManager.h" },
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
		{ Z_Construct_UClass_UNaniteMLManager, UNaniteMLManager::StaticClass, TEXT("UNaniteMLManager"), &Z_Registration_Info_UClass_UNaniteMLManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNaniteMLManager), 2088772270U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_561855607(TEXT("/Script/MLBasedLODSelector"),
	Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_NaniteMLManager_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
