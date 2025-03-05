// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MLBasedLODSelector/Public/DataLogger.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDataLogger() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
MLBASEDLODSELECTOR_API UClass* Z_Construct_UClass_UDataLogger();
MLBASEDLODSELECTOR_API UClass* Z_Construct_UClass_UDataLogger_NoRegister();
UPackage* Z_Construct_UPackage__Script_MLBasedLODSelector();
// End Cross Module References

// Begin Class UDataLogger Function StartLogging
struct Z_Construct_UFunction_UDataLogger_StartLogging_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "logger" },
		{ "ModuleRelativePath", "Public/DataLogger.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDataLogger_StartLogging_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UDataLogger, nullptr, "StartLogging", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDataLogger_StartLogging_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDataLogger_StartLogging_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UDataLogger_StartLogging()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDataLogger_StartLogging_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDataLogger::execStartLogging)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StartLogging();
	P_NATIVE_END;
}
// End Class UDataLogger Function StartLogging

// Begin Class UDataLogger Function StopLogging
struct Z_Construct_UFunction_UDataLogger_StopLogging_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "logger" },
		{ "ModuleRelativePath", "Public/DataLogger.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UDataLogger_StopLogging_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UDataLogger, nullptr, "StopLogging", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UDataLogger_StopLogging_Statics::Function_MetaDataParams), Z_Construct_UFunction_UDataLogger_StopLogging_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UDataLogger_StopLogging()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UDataLogger_StopLogging_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UDataLogger::execStopLogging)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->StopLogging();
	P_NATIVE_END;
}
// End Class UDataLogger Function StopLogging

// Begin Class UDataLogger
void UDataLogger::StaticRegisterNativesUDataLogger()
{
	UClass* Class = UDataLogger::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "StartLogging", &UDataLogger::execStartLogging },
		{ "StopLogging", &UDataLogger::execStopLogging },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDataLogger);
UClass* Z_Construct_UClass_UDataLogger_NoRegister()
{
	return UDataLogger::StaticClass();
}
struct Z_Construct_UClass_UDataLogger_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * UDataLogger\n * - CSV/JSON \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd ML\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xce\xb1\xef\xbf\xbd\n * - \xef\xbf\xbd\xcc\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd (static Get())\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xf0\xbc\xad\xb5\xef\xbf\xbd \xc8\xa3\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n */" },
#endif
		{ "IncludePath", "DataLogger.h" },
		{ "ModuleRelativePath", "Public/DataLogger.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UDataLogger\n- CSV/JSON \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd ML\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xce\xb1\xef\xbf\xbd\n- \xef\xbf\xbd\xcc\xb1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd (static Get())\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xf0\xbc\xad\xb5\xef\xbf\xbd \xc8\xa3\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UDataLogger_StartLogging, "StartLogging" }, // 751314773
		{ &Z_Construct_UFunction_UDataLogger_StopLogging, "StopLogging" }, // 816768730
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDataLogger>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UDataLogger_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_MLBasedLODSelector,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDataLogger_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UDataLogger_Statics::ClassParams = {
	&UDataLogger::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDataLogger_Statics::Class_MetaDataParams), Z_Construct_UClass_UDataLogger_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UDataLogger()
{
	if (!Z_Registration_Info_UClass_UDataLogger.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDataLogger.OuterSingleton, Z_Construct_UClass_UDataLogger_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UDataLogger.OuterSingleton;
}
template<> MLBASEDLODSELECTOR_API UClass* StaticClass<UDataLogger>()
{
	return UDataLogger::StaticClass();
}
UDataLogger::UDataLogger(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UDataLogger);
UDataLogger::~UDataLogger() {}
// End Class UDataLogger

// Begin Registration
struct Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UDataLogger, UDataLogger::StaticClass, TEXT("UDataLogger"), &Z_Registration_Info_UClass_UDataLogger, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDataLogger), 2664175517U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_2526499973(TEXT("/Script/MLBasedLODSelector"),
	Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
