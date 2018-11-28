// Copyright Ben Tristem 2016.

#pragma once
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"

/**
 * 
 */
class BUILDINGESCAPE_API NoLookTeleportation
{
public:
	NoLookTeleportation();
	~NoLookTeleportation();

	float DebugReach = 100.f;
};
