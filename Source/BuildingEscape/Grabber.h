// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


struct FPlayerViewPoint {
	FVector PlayerLocation;
	FRotator PlayerRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere) float Reach = 200.f;
	UPROPERTY() UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY() UInputComponent* Input = nullptr;

	void FindPhysicsHandle();
	void SetupInputComponent();
	void Grab();
	void Release();
	
	// Return the first actor within reach with a physics body.
	FHitResult GetPhysicsBodyInReach() const;

	// Get the vector based on user location and view rotation to determine the player's reach on the level
	FVector GetPlayersReach() const;

	// Get player's viewpoint
	FPlayerViewPoint GetPlayerViewPoint() const;

	// Draw a line that can be seen for debug purposes. Used only for testing the line trace reach when running the game. Not to be used in production.
	void DrawReachLineForTesting() const;
};
