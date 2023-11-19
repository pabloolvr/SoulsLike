// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/CharacterStates.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;

UCLASS()
class SOULSLIKE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;
    
    UPROPERTY(BlueprintReadOnly, Category = Character)
    class APlayerCharacter* PlayerCharacter;
    
    UPROPERTY(BlueprintReadOnly, Category = Movement)
    class UCharacterMovementComponent* CharacterMovement;

    UPROPERTY(BlueprintReadOnly, Category = Movement)
    float GroundSpeed;

    UPROPERTY(BlueprintReadOnly, Category = Movement)
    bool IsBackstepping;

    UPROPERTY(BlueprintReadOnly, Category = Movement)
    bool IsRolling;

    UPROPERTY(BlueprintReadOnly, Category = Movement)
    bool IsSprinting;

    UPROPERTY(BlueprintReadOnly, Category = "Movement | Weapon Equip State")
    EWeaponEquipState WeaponEquipState;
};
