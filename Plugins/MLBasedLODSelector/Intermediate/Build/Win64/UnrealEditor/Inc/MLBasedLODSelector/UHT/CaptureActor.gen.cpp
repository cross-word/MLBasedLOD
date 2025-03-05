// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MLBasedLODSelector/Public/CaptureActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCaptureActor() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_USceneCaptureComponent2D_NoRegister();
MLBASEDLODSELECTOR_API UClass* Z_Construct_UClass_ACaptureActor();
MLBASEDLODSELECTOR_API UClass* Z_Construct_UClass_ACaptureActor_NoRegister();
UPackage* Z_Construct_UPackage__Script_MLBasedLODSelector();
// End Cross Module References

// Begin Class ACaptureActor Function CaptureAndLogMultipleLOD
struct Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Capture" },
		{ "ModuleRelativePath", "Public/CaptureActor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACaptureActor, nullptr, "CaptureAndLogMultipleLOD", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD_Statics::Function_MetaDataParams), Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(ACaptureActor::execCaptureAndLogMultipleLOD)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CaptureAndLogMultipleLOD();
	P_NATIVE_END;
}
// End Class ACaptureActor Function CaptureAndLogMultipleLOD

// Begin Class ACaptureActor
void ACaptureActor::StaticRegisterNativesACaptureActor()
{
	UClass* Class = ACaptureActor::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CaptureAndLogMultipleLOD", &ACaptureActor::execCaptureAndLogMultipleLOD },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACaptureActor);
UClass* Z_Construct_UClass_ACaptureActor_NoRegister()
{
	return ACaptureActor::StaticClass();
}
struct Z_Construct_UClass_ACaptureActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * ACaptureActor\n * - SceneCaptureComponent2D\xeb\xa5\xbc \xed\x86\xb5\xed\x95\xb4 LOD\xeb\xb3\x84 \xed\x99\x94\xeb\xa9\xb4 \xec\xba\xa1\xec\xb2\x98\n * - \xeb\x8f\x99\xec\x8b\x9c\xec\x97\x90 UDataLogger\xeb\xa1\x9c CSV \xeb\xa1\x9c\xea\xb9\x85\n */" },
#endif
		{ "IncludePath", "CaptureActor.h" },
		{ "ModuleRelativePath", "Public/CaptureActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "ACaptureActor\n- SceneCaptureComponent2D\xeb\xa5\xbc \xed\x86\xb5\xed\x95\xb4 LOD\xeb\xb3\x84 \xed\x99\x94\xeb\xa9\xb4 \xec\xba\xa1\xec\xb2\x98\n- \xeb\x8f\x99\xec\x8b\x9c\xec\x97\x90 UDataLogger\xeb\xa1\x9c CSV \xeb\xa1\x9c\xea\xb9\x85" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SceneCapture_MetaData[] = {
		{ "Category", "Capture" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// SceneCaptureComponent\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CaptureActor.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "SceneCaptureComponent" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MaxLOD_MetaData[] = {
		{ "Category", "Capture" },
		{ "ModuleRelativePath", "Public/CaptureActor.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCapture;
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaxLOD;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_ACaptureActor_CaptureAndLogMultipleLOD, "CaptureAndLogMultipleLOD" }, // 3254842150
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACaptureActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACaptureActor_Statics::NewProp_SceneCapture = { "SceneCapture", nullptr, (EPropertyFlags)0x001000000008000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaptureActor, SceneCapture), Z_Construct_UClass_USceneCaptureComponent2D_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SceneCapture_MetaData), NewProp_SceneCapture_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACaptureActor_Statics::NewProp_MaxLOD = { "MaxLOD", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACaptureActor, MaxLOD), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MaxLOD_MetaData), NewProp_MaxLOD_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ACaptureActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaptureActor_Statics::NewProp_SceneCapture,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACaptureActor_Statics::NewProp_MaxLOD,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACaptureActor_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ACaptureActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_MLBasedLODSelector,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACaptureActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ACaptureActor_Statics::ClassParams = {
	&ACaptureActor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_ACaptureActor_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_ACaptureActor_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACaptureActor_Statics::Class_MetaDataParams), Z_Construct_UClass_ACaptureActor_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ACaptureActor()
{
	if (!Z_Registration_Info_UClass_ACaptureActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACaptureActor.OuterSingleton, Z_Construct_UClass_ACaptureActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ACaptureActor.OuterSingleton;
}
template<> MLBASEDLODSELECTOR_API UClass* StaticClass<ACaptureActor>()
{
	return ACaptureActor::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ACaptureActor);
ACaptureActor::~ACaptureActor() {}
// End Class ACaptureActor

// Begin Registration
struct Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACaptureActor, ACaptureActor::StaticClass, TEXT("ACaptureActor"), &Z_Registration_Info_UClass_ACaptureActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACaptureActor), 1959283726U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_2808896913(TEXT("/Script/MLBasedLODSelector"),
	Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
