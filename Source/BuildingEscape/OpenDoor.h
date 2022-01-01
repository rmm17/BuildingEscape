// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FindPressurePlate() const;
	void FindAudioComponent();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	UPrimitiveComponent* GetActorComponent(AActor* Actor) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
	float InitialYaw;
	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere) float OpenAngle = -90.f;
	UPROPERTY(EditAnywhere) ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere) float DoorCloseDelay = 0.25f;
	UPROPERTY(EditAnywhere) float DoorOpenInterpolationSpeed = 2.f;
	UPROPERTY(EditAnywhere) float DoorCloseInterpolationSpeed = 2.f;
	UPROPERTY(EditAnywhere) float MassToOpenDoor = 50.f;
	UPROPERTY() UAudioComponent* Audio = nullptr;
};