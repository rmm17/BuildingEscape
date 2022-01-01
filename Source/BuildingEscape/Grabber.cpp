// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT
#define GrabKeyBinding "Grab"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the physics handle is attached move the object we are holding.
	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->SetTargetLocation(GetPlayersReach());

	// Use for testing purposes only: DrawReachLineForTesting();
}

// Check for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Object %s: No physics handle component found."), *GetOwner()->GetName());
}

void UGrabber::SetupInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if (Input)
	{
		Input->BindAction(GrabKeyBinding, IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction(GrabKeyBinding, IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	FVector LineTraceEnd = GetPlayersReach();

	FHitResult HitResult = GetPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the physics handle.
	if (ComponentToGrab)
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Ray-cast out to a certain distance (Reach)
FHitResult UGrabber::GetPhysicsBodyInReach() const 
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerViewPoint().PlayerLocation,
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayersReach() const 
{
	FPlayerViewPoint ViewPoint = GetPlayerViewPoint();
	FVector PlayerRotation = ViewPoint.PlayerRotation.Vector();

	return ViewPoint.PlayerLocation + PlayerRotation * Reach;;
}

FPlayerViewPoint UGrabber::GetPlayerViewPoint() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);

	return FPlayerViewPoint{ PlayerLocation, PlayerRotation };
}

void UGrabber::DrawReachLineForTesting() const
{
	DrawDebugLine(
		GetWorld(),
		GetPlayerViewPoint().PlayerLocation,
		GetPlayersReach(),
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);
}


