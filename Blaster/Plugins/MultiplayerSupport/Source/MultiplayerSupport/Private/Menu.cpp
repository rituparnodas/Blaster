// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"
#include "MultiplayerSupportSubsystem.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UMenu::MenuSetup(int32 PublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	NumOfpublicConnection = PublicConnections;
	MatchType = TypeOfMatch;
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	UWorld* WorldPtr = GetWorld();
	if (WorldPtr)
	{
		APlayerController* PC = WorldPtr->GetFirstPlayerController();
		if (PC)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputModeData);
			PC->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GI = GetGameInstance();
	if (GI) MultiplayerSupportSubsystem = GI->GetSubsystem<UMultiplayerSupportSubsystem>();
	if (MultiplayerSupportSubsystem)
	{
		MultiplayerSupportSubsystem->MultiplayerOnSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSupportSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSupportSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSupportSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSupportSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();

	UWorld* WorldPtr = GetWorld();
	if (WorldPtr)
	{
		APlayerController* PC = WorldPtr->GetFirstPlayerController();
		if (PC)
		{
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData);
			PC->SetShowMouseCursor(false);
		}
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (HostButton) HostButton->OnClicked.AddDynamic(this, &ThisClass::OnClickHostButton);
	if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnClickJoinButton);

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		//if (GEngine)
		//{
			//GEngine->AddOnScreenDebugMessage(
			//	-1,
			//	15.f,
			//	FColor::Yellow,
			//	FString::Printf(TEXT("Session Created Successfully"))
			//);

			UWorld* WorldPtr = GetWorld();
			if (WorldPtr) WorldPtr->ServerTravel(PathToLobby);
		//}
	}
	else
	{
		if (HostButton) HostButton->SetIsEnabled(true);
	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (!MultiplayerSupportSubsystem) return;

	for (auto Result : SessionResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString User = Result.Session.OwningUserName;
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);

		if (SettingsValue.Equals(MatchType))
		{
			MultiplayerSupportSubsystem->JoinSession(Result);
		}
	}
	if (!bWasSuccessful || SessionResults.Num() <= 0)
	{
		if (JoinButton) JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessioninterface = OnlineSubsystem->GetSessionInterface();
		if (Sessioninterface)
		{
			FString Address;
			Sessioninterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
			if (PC)
			{
				PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		if (JoinButton) JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::OnClickHostButton()
{
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Blue,
	//		FString::Printf(TEXT("Host Button Clicked"))
	//	);
	//}
	if (HostButton) HostButton->SetIsEnabled(false);
	if (MultiplayerSupportSubsystem) MultiplayerSupportSubsystem->CreateSession(NumOfpublicConnection, MatchType);
}

void UMenu::OnClickJoinButton()
{
	if (JoinButton) JoinButton->SetIsEnabled(false);
	if (MultiplayerSupportSubsystem)
	{
		MultiplayerSupportSubsystem->FindSessions(10000);
	}
}