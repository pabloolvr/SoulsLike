// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    // Create a particle system that we can activate or deactivate
    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
    ParticleSystem->bAutoActivate = false;
    ParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystem->SetTemplate(ParticleAsset.Object);
    }

    // Use a spring arm to give the camera smooth, natural-feeling motion.
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
    SpringArm->TargetArmLength = 400.0f;
    SpringArm->bEnableCameraLag = true;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->CameraLagSpeed = 3.0f;

    // Create a camera and attach to our spring arm
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    // Take control of the default player
    //AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Create an instance of our movement component, and tell it to update our root component.
    //PlayerMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CustomMovementComponent"));
    
    //GetCharacterMovement()->UpdatedComponent = RootComponent;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

    Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
    Hair->SetupAttachment(GetMesh());
    Hair->AttachmentName = FString("head");

    Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
    Eyebrows->SetupAttachment(GetMesh());
    Eyebrows->AttachmentName = FString("head");

    MovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    
    // Make sure that we have a valid PlayerController.
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        // Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            // PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
            Subsystem->ClearAllMappings();

            // Add each mapping context, along with their priority values. Higher values outprioritize lower values.
            Subsystem->AddMappingContext(InputContext, 0);
        }
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FColor::Red, false, -1.f, 0, 1.f);
    //GEngine->AddOnScreenDebugMessage(1, .1f, FColor::Red, GetActorForwardVector().ToString());
    GEngine->AddOnScreenDebugMessage(0, .1f, FColor::Red, GetCharacterMovement()->Velocity.ToString());

    if (bIsRolling)
    {
        //GEngine->AddOnScreenDebugMessage(1, .1f, FColor::Red, FString::Printf(TEXT("Rolling")));              
        AddMovementInput(StartRollDirection, 1.f);
    }
    else if (bIsBackstepping)
    {
        //GEngine->AddOnScreenDebugMessage(1, .1f, FColor::Red, FString::Printf(TEXT("Backstepping")));
        AddMovementInput(GetActorForwardVector(), -.75f);
        AddMovementInput(StartBackstepDirection, .0001f);
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
    {
        EnhancedInputComponent->BindAction(CharacterMovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCharacter);
        EnhancedInputComponent->BindAction(CameraMovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCamera);
        EnhancedInputComponent->BindAction(BackstepAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartBackstep);
        EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartRoll);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
        EnhancedInputComponent->BindAction(RightHandAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::RightHandAttack);
        EnhancedInputComponent->BindAction(LeftHandAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftHandAttack);
    }
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& AxisValue)
{  
    if (bIsRolling || bIsBackstepping)
    {
        //GEngine->AddOnScreenDebugMessage(1, .1f, FColor::Red, FString::Printf(TEXT("Rolling")));
        return;
    }

    const FVector2d MovementVector = AxisValue.Get<FVector2D>();

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
    
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, MovementVector.Y);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::MoveCamera(const FInputActionValue& AxisValue)
{
    const FVector2d LookVector = AxisValue.Get<FVector2D>();

    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::StartBackstep()
{
    if (!GetCharacterMovement()->Velocity.IsZero()) return;

    StartBackstepDirection = GetActorForwardVector();

    bIsBackstepping = true;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Started Backstep")));
}

void APlayerCharacter::StopBackstep()
{
    bIsBackstepping = false;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Stopped Backstep")));
}

void APlayerCharacter::StartRoll(const FInputActionValue& Value)
{
    if (GetCharacterMovement()->Velocity.IsZero() || bIsBackstepping) return;

    StartRollDirection = GetActorForwardVector();

    bIsRolling = true;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Started Roll")));
}

void APlayerCharacter::StopRoll()
{
    bIsRolling = false;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Stopped Roll")));
}

void APlayerCharacter::Sprint()
{
    if (GetCharacterMovement()->Velocity.IsZero()) return;

    if (!bIsBackstepping && !bIsRolling) 
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = MovementSpeed * SprintSpeedMultiplier;
    }
}

void APlayerCharacter::StopSprint()
{
    if (GetCharacterMovement()->Velocity.IsZero()) return;

    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
    bIsSprinting = false;
}

void APlayerCharacter::Interact()
{
    AWeapon* Weapon = Cast<AWeapon>(ItemOnRange);

    if (Weapon)
    {
        if (!EquippedRightWeapon)
        {
            EquipOneHandedWeapon(Weapon, true);
            return;
        }
        
        if (!EquippedLeftWeapon)
        {
            EquipOneHandedWeapon(Weapon, false);
            return;
        }
    }
}

void APlayerCharacter::RightHandAttack()
{
    if (ActionState == EActionState::ECS_Unoccupied) 
    {
        PlayAttackMontage(true);
        ActionState = EActionState::ECS_Attacking;
    }
}

void APlayerCharacter::LeftHandAttack()
{
    if (ActionState == EActionState::ECS_Unoccupied)
    {
        PlayAttackMontage(false);
        ActionState = EActionState::ECS_Attacking;
    }
}

void APlayerCharacter::StopAttack()
{
    ActionState = EActionState::ECS_Unoccupied;
}

void APlayerCharacter::PlayAttackMontage(bool bOnRightHand)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    UAnimMontage* AttackMontage = bOnRightHand ? RightHandAttackMontage : LeftHandAttackMontage;

    if (AnimInstance && AttackMontage)
    {
        AnimInstance->Montage_Play(AttackMontage);
        AnimInstance->Montage_JumpToSection(FName("Attack1"), AttackMontage);
    }
}

void APlayerCharacter::ParticleToggle()
{
    if (ParticleSystem && ParticleSystem->Template)
    {
        ParticleSystem->ToggleActive();
    }
}

void APlayerCharacter::EquipOneHandedWeapon(AWeapon* Weapon, bool bOnRightHand)
{
    if (Weapon)
    {
        if (bOnRightHand)
        {
            Weapon->Equip(GetMesh(), "socket_hand_r");
            EquippedRightWeapon = Weapon;
            WeaponEquipState = EquippedLeftWeapon? EWeaponEquipState::ECS_EquippedDualWield : EWeaponEquipState::ECS_EquippedRightHandedWeapon;
        }
        else
        {
            Weapon->Equip(GetMesh(), "socket_hand_l");
            EquippedLeftWeapon = Weapon;
            WeaponEquipState = EquippedRightWeapon ? EWeaponEquipState::ECS_EquippedDualWield : EWeaponEquipState::ECS_EquippedRightHandedWeapon;
        }

    }
}


