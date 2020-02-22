// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "JoinServerListWidget.generated.h"
class UUniformGridPanel;
class UButton;
class UJoinServerSingleWidget;
class UNetworkSessionComponent;
/**
 This is the widget showing all the possible sessions for the owner to join
 */
UCLASS()
class SERVERLISTPROJECT_API UJoinServerListWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UUniformGridPanel* JoinListPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* JoinButton;
	//this is the major component we use to execute any session related command
	UPROPERTY()
		UNetworkSessionComponent* MySessionComponent;
	// the class the children sessions should be
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UJoinServerSingleWidget>SingleJoinSessionClass;
	// currently selected children. We will join games based on which children is currently selected
	UPROPERTY(EditDefaultsOnly)
	UJoinServerSingleWidget* CurrentSelectedWidget;
protected:
	virtual void NativeConstruct() override;
	//listening to Owner's network session component and called when the find session is successful and populate joinlistpanel with the results
	void PopulateJoinList(TArray<FOnlineSessionSearchResult> Results);
	//listening to all the children sessions. If one of them is clicked, then we change the current selected widget and update the children's visual
	UFUNCTION()
	void OnListSelectionChanged(UJoinServerSingleWidget* SelectedWidget);
	UFUNCTION()
		void OnJoinClicked();
};
