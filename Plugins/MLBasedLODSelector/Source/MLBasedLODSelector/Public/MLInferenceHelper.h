#pragma once

#include "CoreMinimal.h"

/*
 머신러닝 inference할때 필요한 데이터를 계산해주는 클래스
*/
class UMLInferenceHelper : public UObject
{
public:
	static UMLInferenceHelper& Get();
	std::vector<float> PreProcessActor(AActor* Actor, UWorld* World, const FMinimalViewInfo& ViewInfo);
	std::vector<float> PreProcessActor(AActor* Actor, UWorld* World);

private:
	float GetActorScreenSize(AActor* TargetActor, UWorld* World);
	float GetNumTriangle(AActor* Actor);
	float GetNumMaterial(AActor* Actor);
	float GetMemoryUsage(AActor* Actor);
};