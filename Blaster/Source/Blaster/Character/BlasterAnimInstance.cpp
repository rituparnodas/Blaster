// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!BlasterCharacter) BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	else
	{
		FVector Velocity = BlasterCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = BlasterCharacter->GetMovementComponent()->IsFalling();

		bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
		bWeaponEquiped = BlasterCharacter->IsWeaponEquiped();
		bIsCrouched = BlasterCharacter->bIsCrouched;
		bIsAiming = BlasterCharacter->IsAiming();
		EquipedWeapon = BlasterCharacter->GetEquipedWeapon();
		TurningInPlace = BlasterCharacter->GetTurningInPlace();

		// This Is Already Replicated So We Don't Need To Replicate
		FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();  
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
		DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
		YawOffset = DeltaRotation.Yaw;

		/*UE_LOG(LogTemp, Warning, TEXT("AimRotation : %f, MovementRotation : %f, DeltaRot : %f, YawOffset : %f"), 
			AimRotation.Yaw,
			MovementRotation.Yaw,
			DeltaRot.Yaw,
			YawOffset
		);*/

		CharacterRotationLastFrame = CharacterRotation;
		CharacterRotation = BlasterCharacter->GetActorRotation();
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
		const float Target = Delta.Yaw / DeltaTime;
		const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
		Lean = FMath::Clamp(Interp, -90.f, 90.f);

		AO_Yaw = BlasterCharacter->GetAO_Yaw();
		AO_Pitch = BlasterCharacter->GetAO_Pitch();

		if (bWeaponEquiped && EquipedWeapon && EquipedWeapon->GetWeaponMesh() && BlasterCharacter->GetMesh())
		{
			LeftHandTransform = EquipedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"));
			FVector OutPosition;
			FRotator OutRotation;
			BlasterCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}
	}
}
