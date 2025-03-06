// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "DataLogger.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef MLBASEDLODSELECTOR_DataLogger_generated_h
#error "DataLogger.generated.h already included, missing '#pragma once' in DataLogger.h"
#endif
#define MLBASEDLODSELECTOR_DataLogger_generated_h

#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execStartLogging);


#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUDataLogger(); \
	friend struct Z_Construct_UClass_UDataLogger_Statics; \
public: \
	DECLARE_CLASS(UDataLogger, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/MLBasedLODSelector"), NO_API) \
	DECLARE_SERIALIZER(UDataLogger)


#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UDataLogger(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UDataLogger(UDataLogger&&); \
	UDataLogger(const UDataLogger&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDataLogger); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDataLogger); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UDataLogger) \
	NO_API virtual ~UDataLogger();


#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_12_PROLOG
#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_INCLASS_NO_PURE_DECLS \
	FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> MLBASEDLODSELECTOR_API UClass* StaticClass<class UDataLogger>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_DataLogger_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
