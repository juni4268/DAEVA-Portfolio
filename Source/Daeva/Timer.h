// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerStage, int32, Stage);

UCLASS()
class DAEVA_API UTimer : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	class AArusa* player;
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	bool IsTimerEnd = false;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Timetext;

	UPROPERTY(BlueprintAssignable, Category = "Timer")
	FOnTimerEnd OnTimerEnd;
	//FOnTimerEnd OnTimerEndOver;


	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	float Maxtime = 180.f;
protected:
	FTimerHandle TimerHandle;
	void UpdateTimer();
	
};
