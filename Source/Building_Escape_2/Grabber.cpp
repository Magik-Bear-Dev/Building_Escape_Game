// Copyright Mark

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Grabber.h"

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
	if (PhysicsHandle)
	{
		// Physics Handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle compoent found on %s!"), *GetOwner()->GetName());
	}
}	

	void UGrabber::Grab()
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));

		GetFirstPhysicsBodyInReach();

		// If we hit something then attach the physics handle 
		// TODO Attach physics handle.
	}
	void UGrabber::Release()
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber released"));
		// TODO remove/realease physics handle.
	}
	
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If physics handle is attached 
		// Move the object we are holding.
		
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get Players viewpoint 
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation, 
		OUT PlayerViewpointRotation
	);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	FHitResult Hit;
	// Ray-cast out to a certain distance (Reach)+
	FCollisionQueryParams TraceParams(FName (TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// Logging out to test
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("LineTrace has hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}
