// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SOULSLIKE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly)
    class APlayerCharacter* PlayerCharacter;
    
    UPROPERTY(BlueprintReadOnly, Category = Movement)
    class UCharacterMovementComponent* PlayerCharacterMovement;

    UPROPERTY(BlueprintReadOnly, Category = Movement)
    float GroundSpeed;
};
