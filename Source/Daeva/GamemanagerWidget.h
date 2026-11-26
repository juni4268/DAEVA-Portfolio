// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamemanagerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DAEVA_API UGamemanagerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void GameOverOn();

	UFUNCTION(BlueprintCallable)
	void GameClearOn();

	UFUNCTION(BlueprintCallable)
	void GameManagerOff();

	UFUNCTION(BlueprintCallable)
	void OnRestart();
	UFUNCTION(BlueprintCallable)
	void OnGameOut();

	void EndKillCount(int32 NewKillCount);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameOver;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameClear;

	UPROPERTY(meta = (BindWidget))
	class UImage* GameManager_Back;

	UPROPERTY(meta = (BindWidget))
	class UButton* Restart;
	UPROPERTY(meta = (BindWidget))
	class UButton* GameOut;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EndPoint;
};
