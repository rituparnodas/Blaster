// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHeadWidget.h"
#include "Components/TextBlock.h"

void UOverHeadWidget::SetDisplayText(FString Text)
{
	if (DisplayText) DisplayText->SetText(FText::FromString(Text));
}

void UOverHeadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole NetRole = InPawn->GetRemoteRole(); 
	FString Role;
	switch (NetRole)
	{
	case ROLE_None: Role = FString("ROLE_None");
		break;
	case ROLE_SimulatedProxy:Role = FString("ROLE_SimulatedProxy");
		break;
	case ROLE_AutonomousProxy:Role = FString("ROLE_AutonomousProxy");
		break;
	case ROLE_Authority:Role = FString("ROLE_Authority");
		break;
	case ROLE_MAX:Role = FString("ROLE_MAX");
		break;
	}
	FString RoleString = FString::Printf(TEXT("Remote Role : %s"), *Role);
	SetDisplayText(RoleString);
}

void UOverHeadWidget::NativeDestruct()
{
	RemoveFromParent();

	Super::NativeDestruct();
}