#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EdFunctionParamNodesSpawner.generated.h"

UCLASS(Config=Editor)
class UEdFunctionParamNodesSpawner : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Config)
	FString ActivationSequence = "a";
	uint32 ActivationProgress = 0;

	FDelegateHandle MouseDownHandle;
	FDelegateHandle KeyDownHandle;

	bool ShouldSpawnParamNodes();
	void ResetSpawnParamNodesConditions();

	void GenerateFuncParamNodes(UBlueprint const* const Blueprint, UEdGraph* graph, const FVector2D& location);

public:
	void StartListeningForInput();
	void StopListeningForInput();
};
