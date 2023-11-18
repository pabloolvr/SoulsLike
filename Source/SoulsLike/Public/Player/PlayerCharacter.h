// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "PlayerCharacter.generated.h"

class UParticleSystemComponent;
class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class AItem;

UCLASS()
class SOULSLIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	APlayerCharacter();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = Input)
    bool IsBackstepping();

    UFUNCTION(BlueprintCallable, Category = Input)
    void StopBackstep();

    UFUNCTION(BlueprintCallable, Category = Input)
    bool IsRolling();

    UFUNCTION(BlueprintCallable, Category = Input)
    void StopRoll();

    UFUNCTION(BlueprintCallable, Category = Input)
    bool IsSprinting();

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = Components)
    UParticleSystemComponent* ParticleSystem;

    UPROPERTY(VisibleAnywhere, Category = Components)
    UCharacterMovementComponent* PlayerMovementComponent;

    UPROPERTY(VisibleAnywhere, Category = Components)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, Category = Components)
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, Category = Components)
    UGroomComponent* Hair;

    UPROPERTY(VisibleAnywhere, Category = Components)
    UGroomComponent* Eyebrows;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputMappingContext* InputContext;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* CharacterMovementAction;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* CameraMovementAction;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* RollAction;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* BackstepAction;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* InteractAction;

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    FVector StartRollDirection;

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    FVector StartBackstepDirection;

    UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
    AItem* ItemOnRange;

    UPROPERTY(EditAnywhere, Category = Gameplay)
    float SprintSpeedMultiplier;

    UPROPERTY()
    float MovementSpeed;

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    bool bIsRolling;

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    bool bIsBackstepping;

    void MoveCharacter(const FInputActionValue& AxisValue);
    void MoveCamera(const FInputActionValue& AxisValue);
    void StartBackstep();
    void StartRoll(const FInputActionValue& Value);
    void Sprint();
    void StopSprint();
    void Interact();
    
    void ParticleToggle();

public:
    FORCEINLINE void SetItemOnRange(AItem* Item) { ItemOnRange = Item; }
    FORCEINLINE AItem* GetItemOnRange() const { return ItemOnRange; }
};
