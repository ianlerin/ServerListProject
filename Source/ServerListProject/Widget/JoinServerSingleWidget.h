// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "JoinServerSingleWidget.generated.h"
class UTextBlock;
class UBorder;
class UButton;
class UJoinServerSingleWidget;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelected, UJoinServerSingleWidget*);
/**
 This widget will represent a single search result and populate the join server list
 */
UCLASS()
class SERVERLISTPROJECT_API UJoinServerSingleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnSelected OnSelectedDelegate;
protected:
	//this is the search result that this widget represents
	FOnlineSessionSearchResult MySearchResult;
	UPROPERTY(EditDefaultsOnly)
		FLinearColor HighlghtedColor;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UBorder* TextBorder;
	// session name is derived from search result
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* MySessionName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* PressButton;

	FString SessionName;
public:
	//should be called when this widget is first being created, to record the search result this widget represents
	void Setup(FOnlineSessionSearchResult SearchResultToSet);
	//called to update visually whether this widget is being selected or not
	void OnSelectionStatusChanged(bool bIsSelected);
	UFUNCTION()
	void OnMousePressed();

	FOnlineSessionSearchResult GetMySearchResult()
	{
		return MySearchResult;
	}
	FString GetMySessionName()
	{
		return SessionName;
	}
	
};
