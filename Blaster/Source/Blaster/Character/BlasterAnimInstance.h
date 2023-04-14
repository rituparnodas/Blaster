// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class ABlasterCharacter* BlasterCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsAccelerating = false;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bWeaponEquiped = false;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		class AWeapon* EquipedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsCrouched = false;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsAiming = false;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		float YawOffset = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		float Lean = 0.f;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;

	FRotator DeltaRotation;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		float AO_Yaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		float AO_Pitch = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		ETurningInPlace TurningInPlace;
};
