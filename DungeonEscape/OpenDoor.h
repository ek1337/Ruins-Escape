// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LowerTheGate(float DeltaTime);
	void UpperTheGate(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

private:
	bool bOpenSoundWasPlayed = false;
	bool bCloseSoundWasPlayed = true;
	
	float DoorInitial_Z;
	float DoorCurrent_Z;
	float DoorTarget_Z = -170.f;
	
	UPROPERTY(EditAnywhere)
	float OpeningSpeed = 30.f;
	
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.5f;

	UPROPERTY(EditAnywhere)
	float RequiredMassToOpenDoor = 20.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

};
