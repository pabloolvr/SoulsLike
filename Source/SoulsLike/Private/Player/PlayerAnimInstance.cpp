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
        PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
    }
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    if (PlayerCharacterMovement)
    {
        GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
    }
}
