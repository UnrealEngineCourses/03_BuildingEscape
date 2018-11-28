// Copyright Ben Tristem 2016.

#pragma once
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "GrabberComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//reach variables for the player 
	float reach = 200.f;
	float reachMAX = 500.f;
	float reachMIN = 85.f;
	float reachChange = 5.f;

	UPhysicsHandleComponent* Physicshandle = nullptr;

	UInputComponent* InputComponent = nullptr;
	
	// Ray-cast and grab what's in reach
	void Grab();

	//Called when grab is released
	void Released();

	//brings the grabbed component closer to the player
	void MoveGrabbedCloser();

	//moves the grabbed component farther away from the player
	void MoveGrabbedAway();

	//Called at the start
	void FindPhysicsHandleComponent();

	//Set (assumed) attached Input Component
	void SetupInputComponent();

	//return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns current Start of reachline
	FVector GetReachLineStart();

	//Returns current end of reachline
	FVector GetReachLineEnd();
};
