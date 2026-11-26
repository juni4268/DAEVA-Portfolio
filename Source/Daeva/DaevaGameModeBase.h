// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DaevaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DAEVA_API ADaevaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTimer> TimerWidgetClass;
	UTimer* TimerWidgetRef;
	UFUNCTION()
	void OnTimerEnded();

	//UFUNCTION()
	//void OnTimerEndedOver();


	UPROPERTY()
	class AArusa* player;

};
