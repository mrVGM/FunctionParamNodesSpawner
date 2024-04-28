#include "EdFunctionParamNodesSpawner.h"

#include "GraphEditor.h"
#include "EdGraphSchema_K2.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_VariableGet.h"
#include "BlueprintVariableNodeSpawner.h"

void UEdFunctionParamNodesSpawner::GenerateFuncParamNodes(UBlueprint const* const Blueprint, UEdGraph* graph, const FVector2D& location)
{
	TArray<UK2Node_FunctionEntry*> GraphEntryNodes;
	graph->GetNodesOfClass<UK2Node_FunctionEntry>(GraphEntryNodes);

	for (UK2Node_FunctionEntry* FunctionEntry : GraphEntryNodes)
	{
		UFunction* SkeletonFunction = FindUField<UFunction>(Blueprint->SkeletonGeneratedClass, graph->GetFName());

		if (SkeletonFunction != nullptr)
		{
			FVector2D cur = location;
			for (TFieldIterator<FProperty> ParamIt(SkeletonFunction); ParamIt && (ParamIt->PropertyFlags & CPF_Parm); ++ParamIt)
			{
				FProperty* Param = *ParamIt;
				const bool bIsFunctionInput = !Param->HasAnyPropertyFlags(CPF_ReturnParm) && (!Param->HasAnyPropertyFlags(CPF_OutParm) || Param->HasAnyPropertyFlags(CPF_ReferenceParm));
				if (bIsFunctionInput)
				{
					UBlueprintNodeSpawner* GetVarSpawner = UBlueprintVariableNodeSpawner::CreateFromMemberOrParam(UK2Node_VariableGet::StaticClass(), Param, graph);
					IBlueprintNodeBinder::FBindingSet bindings;
					GetVarSpawner->Invoke(graph, bindings, cur);
					cur += FVector2D(0, 30);
				}
			}
		}
	}
}

void UEdFunctionParamNodesSpawner::StartListeningForInput()
{
	FSlateApplication* slateApp = &FSlateApplication::Get();

	KeyDownHandle = slateApp->OnApplicationPreInputKeyDownListener().AddLambda([this](const FKeyEvent& keyEvent) {
		if (ActivationSequence.Len() == 0 || ActivationProgress == ActivationSequence.Len())
		{
			ActivationProgress = 0;
			return;
		}

		FString next = FString::Chr(ActivationSequence[ActivationProgress]).ToUpper();
		FString keyName = keyEvent.GetKey().GetFName().ToString();

		if (keyName.Equals(next))
		{
			++ActivationProgress;
			return;
		}

		ActivationProgress = 0;
	});

	MouseDownHandle = slateApp->OnApplicationMousePreInputButtonDownListener().AddLambda([this, slateApp](const FPointerEvent& pointerEvent) {
		bool shouldSpawnParams = ShouldSpawnParamNodes();
		ResetSpawnParamNodesConditions();

		if (!pointerEvent.GetEffectingButton().GetFName().IsEqual("LeftMouseButton"))
		{
			return;
		}

		if (!shouldSpawnParams)
		{
			return;
		}

		TSharedPtr<FSlateUser> user = slateApp->GetUser(pointerEvent);
		FWeakWidgetPath weakWidgetsPath = user->GetLastWidgetsUnderCursor();

		if (!weakWidgetsPath.IsValid())
		{
			return;
		}

		for (int i = 0; i < weakWidgetsPath.Widgets.Num(); ++i)
		{
			TWeakPtr<SWidget> cur = weakWidgetsPath.Widgets[i];
			if (!cur.IsValid())
			{
				continue;
			}

			SWidget* widget = cur.Pin().Get();
			FName type = widget->GetType();

			if (!type.IsEqual("SGraphEditor"))
			{
				continue;
			}

			SGraphEditor* graphEditor = static_cast<SGraphEditor*>(widget);
			UEdGraph* edGraph = graphEditor->GetCurrentGraph();
			const UEdGraphSchema* schema = edGraph->GetSchema();
			const UEdGraphSchema_K2* schemaK2 = Cast<const UEdGraphSchema_K2>(schema);

			if (!schemaK2)
			{
				continue;
			}

			UBlueprint* bp = FBlueprintEditorUtils::FindBlueprintForGraph(edGraph);

			if (!bp)
			{
				continue;
			}

			GenerateFuncParamNodes(bp, edGraph, graphEditor->GetPasteLocation());
		}
	});
}

void UEdFunctionParamNodesSpawner::StopListeningForInput()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication& slateApp = FSlateApplication::Get();
		slateApp.OnApplicationPreInputKeyDownListener().Remove(KeyDownHandle);
		slateApp.OnApplicationMousePreInputButtonDownListener().Remove(MouseDownHandle);
	}
}


bool UEdFunctionParamNodesSpawner::ShouldSpawnParamNodes()
{
	return ActivationSequence.Len() > 0 && ActivationProgress == ActivationSequence.Len();
}

void UEdFunctionParamNodesSpawner::ResetSpawnParamNodesConditions()
{
	ActivationProgress = 0;
}