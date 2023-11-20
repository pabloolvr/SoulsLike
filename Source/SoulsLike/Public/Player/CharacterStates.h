#pragma once

UENUM(BlueprintType)
enum class EWeaponEquipState : uint8
{
    ECS_Unequipped UMETA(DisplayName = "Unequipped"),
    ECS_EquippedRightHandedWeapon UMETA(DisplayName = "Equipped Right-Handed Weapon"),
    ECS_EquippedLeftHandedWeapon UMETA(DisplayName = "Equipped Left-Handed Weapon"),
    ECS_EquippedDualWield UMETA(DisplayName = "Equipped Dual wielding"),
    ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
    ECS_Idle UMETA(DisplayName = "Idle"),
    ECS_Attacking UMETA(DisplayName = "Attacking"),
    ECS_Blocking UMETA(DisplayName = "Blocking"),
    ECS_Rolling UMETA(DisplayName = "Rolling"),
    ECS_Backstepping UMETA(DisplayName = "Backstepping"),
    ECS_Sprinting UMETA(DisplayName = "Sprinting"),
};