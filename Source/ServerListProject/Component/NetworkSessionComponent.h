// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "Components/ActorComponent.h"
#include "NetworkSessionComponent.generated.h"
/*
this component is responsible for all creating, finding and joining session for the owner character
*/

const static FName SESSION_KEY = TEXT("Game");
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFoundSessionsComplete, TArray<FOnlineSessionSearchResult>);

DECLARE_MULTICAST_DELEGATE(FOnJoinSuccessful);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERVERLISTPROJECT_API UNetworkSessionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FOnFoundSessionsComplete OnFoundSessionsCompleteDelegate;
	FOnJoinSuccessful OnJoinSuccessfulDelegate;
protected:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
public:	
	// Sets default values for this component's properties
	UNetworkSessionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//delegate  function called when find session is completed
	void OnFindServerListComplete(bool Success);

	//delegate  function called when join session is completed
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//delegate  function called when create session is completed
	void OnCreateSession(FName SessionName, bool Success);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// create a new session.Called from ServerListWidget
	UFUNCTION(BlueprintCallable)
	void Host(int32 NetID, FName SessionToCreate);
	// find all existing session.Called from JoinServerList
	UFUNCTION(BlueprintCallable)
		void FindServerList();
	//join a session. Called from JoinServerList
	void Join(FOnlineSessionSearchResult SessionToJoin, FName SessionName);

};
