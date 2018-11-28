// Copyright Ben Tristem 2016.

#include "BuildingEscape.h"
#include "GrabberComponent.h"

#define OUT
// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


void UGrabberComponent::Grab()
{
	//LINE TRACE and see if we reach any actors with physics body collision Channel Set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	//If we hit something then attach a physics handle
	if (ActorHit)
	{ 
		if (!Physicshandle) { return; }
		Physicshandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
		
	}	
}

void UGrabberComponent::Released()
{	
	if (!Physicshandle) { return; }
	Physicshandle->ReleaseComponent();
	reach = 200.f;
}

void UGrabberComponent::MoveGrabbedCloser()
{
	if (!Physicshandle) { return; }
	//the Grabbedcomponent should be able to be moved closer no matter how far away it is
	if (Physicshandle->GrabbedComponent && reach > reachMIN)
	{
		reach = reach - reachChange;
	}
}

void UGrabberComponent::MoveGrabbedAway()
{
	if (!Physicshandle) { return; }
	//the Grabbedcomponent should be able to be moved away no matter how close it is
	if(Physicshandle->GrabbedComponent && reach < reachMAX)
	{
		reach = reach + reachChange;
	}
}

///look for attached Physics Handle
void UGrabberComponent::FindPhysicsHandleComponent()
{
	
	Physicshandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Physicshandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Physics handle"), *GetOwner()->GetName());
	}
}

///look for attached Input component(only appears at Runtime)
void UGrabberComponent::SetupInputComponent()
{	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberComponent::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberComponent::Released);
		InputComponent->BindAction("GrabDistanceUP",IE_Pressed, this, &UGrabberComponent::MoveGrabbedAway);
		InputComponent->BindAction("GrabDistanceDOWN", IE_Pressed, this, &UGrabberComponent::MoveGrabbedCloser);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Input Component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Physicshandle) { return; }
	//if the physics handle is attached
	if (Physicshandle->GrabbedComponent)
	{
		//move the object were holding
		Physicshandle->SetTargetLocation(GetReachLineEnd());
	}		
}

const FHitResult UGrabberComponent::GetFirstPhysicsBodyInReach()
{
	//setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	//Line Trace (AKA Ray-Cast) out to reach distance
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType
	(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}

FVector UGrabberComponent::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabberComponent::GetReachLineEnd()
{		
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;	

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint	
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * reach);		
}