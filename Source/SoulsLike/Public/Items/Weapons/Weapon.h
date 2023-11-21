// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class SOULSLIKE_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    void Equip(USceneComponent* Parent, FName SocketName);
    void Unequip();

    FORCEINLINE bool IsEquipped() const { return bIsEquipped; }
    FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
    
    UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    bool bIsEquipped = false;

    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    UBoxComponent* CollisionBox;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceStart;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* BoxTraceEnd;
};
