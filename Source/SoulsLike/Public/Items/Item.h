// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SOULSLIKE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
    AItem();

    virtual void Tick(float DeltaTime) override;
protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
    float Amplitude = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
    float TimeConstant = 5.f;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ItemMesh;

    UFUNCTION(BlueprintPure)
    float TransformedSin();

    UFUNCTION(BlueprintPure)
    float TransformedCos();

    template<typename T>
    T Avg(T First, T Second);

    UFUNCTION()
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float RunningTime;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* Sphere;
};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
    return (First + Second) / 2;
}