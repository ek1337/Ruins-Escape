// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float Reach = 200.f;
	
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void FindPhysicsHandle();
	void SetUpInputComponent();
	void Grab();
	void Release();

	// Return the first Actor within reach with PhysicsBody
	FHitResult GetFirstPhysicsBodyInReach() const;
	// return the vector's end of our line trace
	FVector GetPlayersReach() const;
	// return player's position in the world
	FVector GetPlayersWorldPos() const;
	
};
