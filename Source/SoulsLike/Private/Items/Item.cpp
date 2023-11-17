// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = true;
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    RootComponent = ItemMesh;
    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(GetRootComponent());
}
void AItem::BeginPlay()
{
    Super::BeginPlay();

    // add start/end overlap events
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
    Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}
float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const FString OtherActorName = FString("Begin Overlap with: ") + OtherActor->GetName();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
    }
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    const FString OtherActorName = FString("End Overlap with: ") + OtherActor->GetName();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
    }
}

void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RunningTime += DeltaTime;
}
