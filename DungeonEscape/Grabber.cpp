// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetUpInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if physics handle is attached (if we actually grabbed something)
	if (!PhysicsHandle) // protect against nullptr
	{
		return;
	}
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("The component PhysicsHandle in class %s was NOT found!"), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else UE_LOG(LogTemp, Error, TEXT("The component InputComponent in class %s was NOT found!"), *GetOwner()->GetName());
}

void UGrabber::Grab()
{
	// only ray cast when key is pressed and see if we reach any actor with a Physics Body Collision Channel set.
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// + lock component from rotating when grabbed
	const FRotator ZeroRotation = {0,0,0};
	
	//if we hit something, then attach the physics handle
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		if (!PhysicsHandle) // protect against nullptr
		{
			return;
		}
		PhysicsHandle->GrabComponentAtLocationWithRotation(
		ComponentToGrab,
		NAME_None,
		GetPlayersReach(),
		ZeroRotation
	);
	}	
}

void UGrabber::Release()
{
	if (!PhysicsHandle) // protect against nullptr
	{
		return;
	}
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// ray-cast out to a certain distance (reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPoint_Location;
	FRotator PlayerViewPoint_Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint_Location,
		OUT PlayerViewPoint_Rotation
	);
	return PlayerViewPoint_Location + PlayerViewPoint_Rotation.Vector() * Reach;;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPoint_Location;
	FRotator PlayerViewPoint_Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint_Location,
		OUT PlayerViewPoint_Rotation
	);

	return PlayerViewPoint_Location;
}

