// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
    PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
    if (PlayerCharacter) 
    {
        CharacterMovement = PlayerCharacter->GetCharacterMovement();
    }
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    if (CharacterMovement)
    {
        GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
        IsBackstepping = PlayerCharacter->IsBackstepping();
        IsRolling = PlayerCharacter->IsRolling();
        IsSprinting = PlayerCharacter->IsSprinting();
        CharacterState = PlayerCharacter->GetCharacterState();
    }
}

void UPlayerAnimInstance::OnBackstepEnd()
{
    PlayerCharacter->StopBackstep();
}

void UPlayerAnimInstance::OnRollEnd()
{
    PlayerCharacter->StopRoll();
}
