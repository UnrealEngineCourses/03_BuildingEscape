// Copyright Ben Tristem 2016.


#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	FindPressurePlate();
	FindOwner();

	Owner = GetOwner();

}

void UOpenDoor::FindPressurePlate()
{
	if (PressurePlate == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Pressure Plate"), *GetOwner()->GetName());

	}
}

void UOpenDoor::FindOwner()
{
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Owner"), *GetOwner()->GetName());

	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) //TODO make into Parameter
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float Totalmass = 0.f;

	TArray<AActor*> OverLappingActors;
	//find all the overlapping actors
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);
	
	//Iterate through all of them adding their masses
		for (const auto* CurrentActor : OverLappingActors)
		{
			Totalmass += CurrentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *CurrentActor->GetName());


		}
	return Totalmass;
}