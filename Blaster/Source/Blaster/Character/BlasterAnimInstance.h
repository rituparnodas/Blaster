// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
		float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bWeaponEquiped;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = "movement", meta = (AllowPrivateAccess = "true"))
		bool bIsAiming;
};
