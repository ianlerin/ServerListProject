// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkSessionComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UNetworkSessionComponent::UNetworkSessionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNetworkSessionComponent::BeginPlay()
{
	Super::BeginPlay();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface)
		{
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkSessionComponent::OnJoinSession);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetworkSessionComponent::OnFindServerListComplete);
		}
	}
	// ...
	
}


// Called every frame
void UNetworkSessionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNetworkSessionComponent::Host(int32 NetID, FName SessionToCreate)
{
	if (!SessionInterface) { return; }
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	FString MyName =SessionToCreate.ToString();
	SessionSettings.Set(SESSION_KEY, MyName, EOnlineDataAdvertisementType::ViaOnlineService);
	//SessionSettings.Set("Server", SessionSettings);

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkSessionComponent::OnCreateSession);
	SessionInterface->CreateSession(0, SessionToCreate, SessionSettings);
}



void UNetworkSessionComponent::OnCreateSession(FName SessionName, bool Success)
{
	SessionInterface->OnCreateSessionCompleteDelegates.RemoveAll(this);
	if (Success)
	{
		SessionInterface->StartSession(SessionName);
		UGameplayStatics::OpenLevel(this, "ThirdPersonExampleMap", true, "listen");
		//GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UNetworkSessionComponent::FindServerList()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("FindServerList")));
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 20;

	SessionSearch->bIsLanQuery = true;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	if (SessionSearch.IsValid())
	{

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UNetworkSessionComponent::OnFindServerListComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));
		//UE_LOG(LogTemp, Warning, TEXT("UNetworkSessionComponent::Finished Find Session:%i"), SessionSearch->SearchResults.Num());
		OnFoundSessionsCompleteDelegate.Broadcast(SessionSearch->SearchResults);

	}
}

void UNetworkSessionComponent::Join(FOnlineSessionSearchResult SessionToJoin,FName SessionName)
{
	SessionInterface->JoinSession(0, SessionName, SessionToJoin);
	
}

void UNetworkSessionComponent::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		return;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0);
	if (PlayerController)
	{

		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		OnJoinSuccessfulDelegate.Broadcast();
	}
}