// Fill out your copyright notice in the Description page of Project Settings.

#include "JoinServerSingleWidget.h"
#include "Component/NetworkSessionComponent.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/Border.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void UJoinServerSingleWidget::Setup(FOnlineSessionSearchResult SearchResultToSet)
{
	
	MySearchResult = SearchResultToSet;
	
	SearchResultToSet.Session.SessionSettings.Get(SESSION_KEY, SessionName);
	MySessionName->SetText(FText::FromString(SessionName));
	PressButton->OnPressed.AddDynamic(this, &UJoinServerSingleWidget::OnMousePressed);

	//MySessionName->SetText(FText::FromString(ServerName));
	//SearchResultToSe
	//MySessionName->SetText(FText::FromString(SearchResultToSet.HostUsername));
	
}
void UJoinServerSingleWidget::OnSelectionStatusChanged(bool bIsSelected)
{
	if (bIsSelected)
	{

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnSelectionStatusChanged,selected")));
		
		TextBorder->SetBrushColor(HighlghtedColor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnSelectionStatusChanged,non selected")));
		
		TextBorder->SetBrushColor(FLinearColor(1, 1, 1, 1));
	}
}
void UJoinServerSingleWidget::OnMousePressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnMousePressed")));
	OnSelectedDelegate.Broadcast(this);
}
