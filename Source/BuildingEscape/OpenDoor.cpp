// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindPressurePlate();
	FindAudioComponent();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	OpenAngle += GetOwner()->GetActorRotation().Yaw;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float TimeInSeconds = GetWorld()->GetTimeSeconds();

	if (TotalMassOfActors() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = TimeInSeconds;
	}
	else if (TimeInSeconds - DoorLastOpened >= DoorCloseDelay)
		CloseDoor(DeltaTime);
}

void UOpenDoor::FindPressurePlate() const 
{
	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no PressurePlate trigger volume set."), *GetOwner()->GetName());
}

void UOpenDoor::FindAudioComponent()
{
	Audio = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!Audio)
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, OpenAngle, DeltaTime, DoorOpenInterpolationSpeed);

	GetOwner()->SetActorRotation(ActorRotation);

	CloseDoorSound = false;
	if (Audio && !OpenDoorSound) 
	{
		Audio->Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, InitialYaw, DeltaTime, DoorCloseInterpolationSpeed);

	GetOwner()->SetActorRotation(ActorRotation);

	OpenDoorSound = false;
	if (Audio && !CloseDoorSound)
	{
		Audio->Play();
		CloseDoorSound = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate)
		return 0.f;

	// Find all overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* Actor : OverlappingActors) {
		UPrimitiveComponent* Component = GetActorComponent(Actor);

		if (!Component)
			continue;

		if (Component->IsSimulatingPhysics())
			TotalMass += Component->GetMass();
	}

	return TotalMass;
}

UPrimitiveComponent* UOpenDoor::GetActorComponent(AActor* Actor) const {
	// Probably ideal to just use FindComponentByClass to avoid casting, but for now remains in the code as an example for casting.
	UActorComponent* Component = Actor->GetComponentByClass(UPrimitiveComponent::StaticClass());

	return Cast<UPrimitiveComponent>(Component);
}
