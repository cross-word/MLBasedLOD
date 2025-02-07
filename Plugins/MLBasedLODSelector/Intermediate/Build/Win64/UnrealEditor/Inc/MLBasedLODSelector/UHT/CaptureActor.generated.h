// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CaptureActor.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef MLBASEDLODSELECTOR_CaptureActor_generated_h
#error "CaptureActor.generated.h already included, missing '#pragma once' in CaptureActor.h"
#endif
#define MLBASEDLODSELECTOR_CaptureActor_generated_h

#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execCaptureAndLogMultipleLOD);


#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesACaptureActor(); \
	friend struct Z_Construct_UClass_ACaptureActor_Statics; \
public: \
	DECLARE_CLASS(ACaptureActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/MLBasedLODSelector"), NO_API) \
	DECLARE_SERIALIZER(ACaptureActor)


#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ACaptureActor(ACaptureActor&&); \
	ACaptureActor(const ACaptureActor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACaptureActor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACaptureActor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ACaptureActor) \
	NO_API virtual ~ACaptureActor();


#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_15_PROLOG
#define FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_INCLASS_NO_PURE_DECLS \
	FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> MLBASEDLODSELECTOR_API UClass* StaticClass<class ACaptureActor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_unreal_project_LODPlugin_Plugins_MLBasedLODSelector_Source_MLBasedLODSelector_Public_CaptureActor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
