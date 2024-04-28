// Copyright Epic Games, Inc. All Rights Reserved.

#include "FunctionParamNodesSpawner.h"

#include "EdFunctionParamNodesSpawner.h"

#define LOCTEXT_NAMESPACE "FFunctionParamNodesSpawnerModule"

void FFunctionParamNodesSpawnerModule::StartupModule()
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	static UEdFunctionParamNodesSpawner* spawner = nullptr;

	if (!spawner)
	{
		spawner = NewObject<UEdFunctionParamNodesSpawner>();

		// This object will live as long as the editor does
		spawner->AddToRoot();
		spawner->StartListeningForInput();
	}

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FFunctionParamNodesSpawnerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFunctionParamNodesSpawnerModule, FunctionParamNodesSpawner)