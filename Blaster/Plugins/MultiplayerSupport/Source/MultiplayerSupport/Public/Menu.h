// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Menu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYERSUPPORT_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Initialize")
		void MenuSetup(int32 PublicConnections = 4, FString TypeOfMatch = TEXT("FreeForAll"), FString LobbyPath = TEXT("/Game/ThirdPerson/Maps/Lobby"));

	void MenuTearDown();

protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
		void OnCreateSession(bool bWasSuccessful);

	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION()
		void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
		void OnStartSession(bool bWasSuccessful);
	
private:

	UPROPERTY(meta = (BindWidget))
		UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		UButton* JoinButton;

	UFUNCTION()
		void OnClickHostButton();

	UFUNCTION()
		void OnClickJoinButton();

	class UMultiplayerSupportSubsystem* MultiplayerSupportSubsystem;

	int32 NumOfpublicConnection{4};
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };
};