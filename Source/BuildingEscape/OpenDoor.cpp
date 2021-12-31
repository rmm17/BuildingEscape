// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

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

	TargetYaw += GetOwner()->GetActorRotation().Yaw;
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no PressurePlate trigger volume set."), *GetOwner()->GetName());
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
		OpenDoor(DeltaTime);
}

void UOpenDoor::OpenDoor(float DeltaTime) const 
{
	FRotator ActorRotation = GetOwner()->GetActorRotation();
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, TargetYaw, DeltaTime, 2);

	GetOwner()->SetActorRotation(ActorRotation);
}
