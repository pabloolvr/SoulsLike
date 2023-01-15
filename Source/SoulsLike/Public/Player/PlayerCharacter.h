// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SOULSLIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY()
    class UParticleSystemComponent* PlayerParticleSystem;

    UPROPERTY()
    class UCharacterMovementComponent* PlayerMovementComponent;

    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void ParticleToggle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
