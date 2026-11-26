// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PotionWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class DAEVA_API UPotionWidget : public UUserWidget
{
	GENERATED_BODY()

	

public:
	UFUNCTION(BlueprintCallable)
	void StartPotionUI(int amount);

	UFUNCTION(BlueprintCallable)	
	void OnPotionUI(int amount);

	UFUNCTION(BlueprintCallable)
	void OffPotionUI(int amount);
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Potion;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Potion_amount;

	UPROPERTY(meta = (BindWidget))
	UImage* NonPotion;
};
