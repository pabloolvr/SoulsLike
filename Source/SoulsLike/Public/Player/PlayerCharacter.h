// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "PlayerCharacter.generated.h"

class UParticleSystemComponent;
class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class SOULSLIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	APlayerCharacter();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = Animation)
    void StopBackstep();

    UFUNCTION(BlueprintCallable, Category = Animation)
    void StopRoll();

    UFUNCTION(BlueprintCallable, Category = Animation)
    void StopAttack();

    FORCEINLINE bool IsBackstepping() const { return ActionState == EActionState::ECS_Backstepping; }
    FORCEINLINE bool IsRolling() const { return ActionState == EActionState::ECS_Rolling; }
    FORCEINLINE bool IsSprinting() const { return ActionState == EActionState::ECS_Sprinting; }

    FORCEINLINE void SetItemOnRange(AItem* Item) { ItemOnRange = Item; }
    FORCEINLINE AItem* GetItemOnRange() const { return ItemOnRange; }

    FORCEINLINE EWeaponEquipState GetWeaponEquipState() const { return WeaponEquipState; }

protected:
	virtual void BeginPlay() override;

private:
    /* Components References */

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

    /* Input Actions */

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

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* RightHandAttackAction;

    UPROPERTY(EditAnywhere, Category = Input)
    UInputAction* LeftHandAttackAction;

    /* Gameplay variables */

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    FVector StartRollDirection;

    UPROPERTY(VisibleAnywhere, Category = Gameplay)
    FVector StartBackstepDirection;

    UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
    AItem* ItemOnRange;

    UPROPERTY(EditAnywhere, Category = Gameplay)
    float SprintSpeedMultiplier;

    UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
    float MovementSpeed;

    UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
    EWeaponEquipState WeaponEquipState = EWeaponEquipState::ECS_Unequipped;

    UPROPERTY(VisibleInstanceOnly, Category = Gameplay)
    EActionState ActionState = EActionState::ECS_Idle;

    /* Equipped Items References */

    UPROPERTY(VisibleInstanceOnly, Category = Equips)
    AWeapon* EquippedRightWeapon;

    UPROPERTY(VisibleInstanceOnly, Category = Equips)
    AWeapon* EquippedLeftWeapon;

    /* Montages */

    UPROPERTY(EditDefaultsOnly, Category = Montages)
    UAnimMontage* RightHandAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = Montages)
    UAnimMontage* LeftHandAttackMontage;

    /* Input Callbacks */
    void MoveCharacter(const FInputActionValue& AxisValue);
    void MoveCamera(const FInputActionValue& AxisValue);
    void StartBackstep();
    void StartRoll(const FInputActionValue& Value);
    void Sprint();
    void StopSprint();
    void Interact();
    void RightHandAttack();
    void LeftHandAttack();   
    void ParticleToggle();

    /* Gameplay functions */
    void EquipOneHandedWeapon(AWeapon* Weapon, bool bOnRightHand);

    /* Montage play functions */
    void PlayAttackMontage(bool bOnRightHand);
};
