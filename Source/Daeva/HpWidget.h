// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HpWidget.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class DAEVA_API UHpWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHPProgressBar(float Current, float Max);
protected:
	//virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HP_Prog;
};
