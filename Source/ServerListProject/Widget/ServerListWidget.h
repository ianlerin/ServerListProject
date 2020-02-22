// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerListWidget.generated.h"
class UUniformGridPanel;
class UButton;
class USizeBox;
class UMultiLineEditableTextBox;
class UWidgetSwitcher;
class UJoinServerListWidget;
class UNetworkSessionComponent;
/**
 * 
 */
UCLASS()
class SERVERLISTPROJECT_API UServerListWidget : public UUserWidget
{
	GENERATED_BODY()
protected:


	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* HostButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* FindButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UWidgetSwitcher* FindHostSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		USizeBox* NameInputSizeBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UJoinServerListWidget* WBP_JoinMenu;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UMultiLineEditableTextBox* NameInput;
	//this is the major component we use to execute any session related command
	UPROPERTY()
		UNetworkSessionComponent* MySessionComponent;
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
		void OnFindPressed();
	UFUNCTION()
		void OnHostPressed();
	UFUNCTION()
	void Remove();
};
