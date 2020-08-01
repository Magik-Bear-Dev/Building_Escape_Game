// Copyright Mark

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"

#define OUT

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
	void UGrabber::SetupInputComponent()
	{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

	// Check for the Physics Handle Component 
	void UGrabber::FindPhysicsHandle()
	{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle compoent found on %s!"), *GetOwner()->GetName());
	}
}	

	void UGrabber::Grab()
	{
		FHitResult  HitResult = GetFirstPhysicsBodyInReach();
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		AActor* ActorHit = HitResult.GetActor();

		// If we hit something then attach the physics handle 
		if (ActorHit)
		{
			if (!PhysicsHandle){return;}
			PhysicsHandle->GrabComponentAtLocation
			(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
			);
		}
	}

	void UGrabber::Release()
	{
		if (!PhysicsHandle){return;}
		PhysicsHandle->ReleaseComponent();
	}
	
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If physics handle is attached 
	if (!PhysicsHandle){return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{	
	FHitResult Hit;
	// Ray-cast out to a certain distance (Reach)+
	FCollisionQueryParams TraceParams(FName (TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, 
		OUT PlayerViewpointRotation
	);

	return PlayerViewpointLocation;
}

	FVector UGrabber::GetPlayersReach() const
	{
		FVector PlayerViewpointLocation;
		FRotator PlayerViewpointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewpointLocation, 
			OUT PlayerViewpointRotation
	);
	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
	}