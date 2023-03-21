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
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
    {
        EnhancedInputComponent->BindAction(CharacterMovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCharacter);
        EnhancedInputComponent->BindAction(CameraMovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCamera);
    }
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& AxisValue)
{
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

void APlayerCharacter::ParticleToggle()
{
    if (ParticleSystem && ParticleSystem->Template)
    {
        ParticleSystem->ToggleActive();
    }
}

void APlayerCharacter::SprintToggle()
{

}

