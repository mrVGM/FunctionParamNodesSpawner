// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityObject.h"
#include "EdFunctionParamNodesSpawner.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UEdFunctionParamNodesSpawner : public UEditorUtilityObject
{
	GENERATED_BODY()

private:
	void GenerateFuncParamNodes(UBlueprint const* const Blueprint, UEdGraph* graph, const FVector2D& location);

public:
	UFUNCTION(BlueprintCallable)
	void StartListeningForInput();

	UFUNCTION(BlueprintImplementableEvent)
	void OnKeyDown(const FKey& key);

	UFUNCTION(BlueprintImplementableEvent)
	bool ShouldSpawnParamNodes();

	UFUNCTION(BlueprintImplementableEvent)
	void ResetSpawnParamNodesConditions();
};
