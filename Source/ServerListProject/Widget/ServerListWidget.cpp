// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerListWidget.h"
#include "Widget/JoinServerListWidget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Component/NetworkSessionComponent.h"
//#include "Widget/JoinSessionListWidget.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "UMG/Public/Components/SizeBox.h"
#include "UMG/Public/Components/MultiLineEditableTextBox.h"
#include "UMG/Public/Components/EditableTextBox.h"
#include "Runtime/UMG/Public/Components/Button.h"
void UServerListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		MySessionComponent = MyPawn->FindComponentByClass<UNetworkSessionComponent>();
		HostButton->OnPressed.AddDynamic(this, &UServerListWidget::OnHostPressed);
		FindButton->OnPressed.AddDynamic(this, &UServerListWidget::OnFindPressed);
		MySessionComponent->OnJoinSuccessfulDelegate.AddUFunction(this, "Remove");
	
	}
}

void UServerListWidget::OnHostPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHostPressed"));
	

	if (FindHostSwitcher->GetActiveWidget() == NameInputSizeBox)
	{
		if (MySessionComponent)
		{
			bool IsLegitText = !NameInput->GetText().IsEmptyOrWhitespace();
			if (!IsLegitText) {
				UE_LOG(LogTemp, Warning, TEXT("OnHostPressed, non legit text")); return;
			}
			UE_LOG(LogTemp, Warning, TEXT("OnHostPressed, Host"));

			MySessionComponent->Host(GetOwningPlayerState()->GetUniqueID(),*NameInput->GetText().ToString());

			Remove();
		}

	}
	else
	{
		FindHostSwitcher->SetActiveWidget(NameInputSizeBox);
		UE_LOG(LogTemp, Warning, TEXT("OnHostPressed,Switch widget"));
	}


}


void UServerListWidget::OnFindPressed()
{

	if (MySessionComponent)
	{
		MySessionComponent->FindServerList();
	}

	FindHostSwitcher->SetActiveWidget(WBP_JoinMenu);


}

void UServerListWidget::Remove()
{
	UE_LOG(LogTemp, Warning, TEXT("UServerListWidget::Remove"));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeGameOnly GameOnlyInput;
		PlayerController->SetInputMode(GameOnlyInput);
		PlayerController->bShowMouseCursor = false;
	}
	RemoveFromParent();
}
