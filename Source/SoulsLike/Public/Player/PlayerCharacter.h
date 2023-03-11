// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UParticleSystemComponent;
class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

UCLASS()
class SOULSLIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	APlayerCharacter();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* ParticleSystem;

    UPROPERTY(VisibleAnywhere)
    UCharacterMovementComponent* PlayerMovementComponent;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere)
    UGroomComponent* Hair;

    UPROPERTY(VisibleAnywhere)
    UGroomComponent* Eyebrows;

    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void Turn(float AxisValue);
    void LookUp(float AxisValue);
    void ParticleToggle();

};
