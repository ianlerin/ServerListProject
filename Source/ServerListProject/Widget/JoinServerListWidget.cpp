// Fill out your copyright notice in the Description page of Project Settings.

#include "JoinServerListWidget.h"
#include "Runtime/UMG/Public/Components/UniformGridSlot.h"
#include "Widget/JoinServerSingleWidget.h"
#include "Runtime/UMG/Public/Components/UniformGridPanel.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Component/NetworkSessionComponent.h"
#include "OnlineSessionSettings.h"
#include "Runtime/UMG/Public/Components/Button.h"
void UJoinServerListWidget::NativeConstruct()
{
	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		MySessionComponent = MyPawn->FindComponentByClass<UNetworkSessionComponent>();
		MySessionComponent->OnFoundSessionsCompleteDelegate.AddUObject(this, &UJoinServerListWidget::PopulateJoinList);
	}
	JoinButton->OnPressed.AddDynamic(this, &UJoinServerListWidget::OnJoinClicked);
}

void UJoinServerListWidget::PopulateJoinList(TArray<FOnlineSessionSearchResult> Results)
{
	JoinListPanel->ClearChildren();
	CurrentSelectedWidget = nullptr;
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("PopulateJoinList")));
	for (int i = 0; i < Results.Num(); i++)
	{
		UJoinServerSingleWidget* SingleWidget = CreateWidget<UJoinServerSingleWidget>(GetOwningPlayer(), SingleJoinSessionClass);
		if (SingleWidget)
		{
			
			SingleWidget->Setup(Results[i]);
			SingleWidget->OnSelectedDelegate.AddUFunction(this, "OnListSelectionChanged");
			UUniformGridSlot* MySlot=JoinListPanel->AddChildToUniformGrid(SingleWidget);
			if (MySlot)
			{
				MySlot->SetRow(i);
			}
		}
	}
}

void UJoinServerListWidget::OnListSelectionChanged(UJoinServerSingleWidget* SelectedWidget)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnListSelectionChanged")));
	if (CurrentSelectedWidget)
	{
		CurrentSelectedWidget->OnSelectionStatusChanged(false);
	}
	CurrentSelectedWidget = SelectedWidget;
	CurrentSelectedWidget->OnSelectionStatusChanged(true);
}

void UJoinServerListWidget::OnJoinClicked()
{
	if (CurrentSelectedWidget)
	{
		MySessionComponent->Join(CurrentSelectedWidget->GetMySearchResult(), *CurrentSelectedWidget->GetMySessionName());
	}
}