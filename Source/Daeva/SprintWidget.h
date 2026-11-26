// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SprintWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class DAEVA_API USprintWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ShowCooltimeUI(float TimeRemaining);

	UFUNCTION(BlueprintCallable)
	void HideCooltimeUI();
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Sprint;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Sprint_CoolTime;

	UPROPERTY(meta = (BindWidget))
	UImage* NonSprint;
};

