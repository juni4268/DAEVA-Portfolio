// Fill out your copyright notice in the Description page of Project Settings.


#include "DaevaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Timer.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Arusa.h"

void ADaevaGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(
		GetWorld(), AArusa::StaticClass());
	player = Cast<AArusa>(actor);

	if (TimerWidgetClass)
	{
		UTimer* TimerWidget = CreateWidget<UTimer>(GetWorld(), TimerWidgetClass);
		if (TimerWidget)
		{
			TimerWidget->AddToViewport();
			TimerWidgetRef = TimerWidget;
			TimerWidget->OnTimerEnd.AddDynamic(this, &ADaevaGameModeBase::OnTimerEnded);
		}
		
	}
}

void ADaevaGameModeBase::OnTimerEnded()
{
	player->GameClear = true;
	player->GameClearManager();
}

//void ADaevaGameModeBase::OnTimerEndedOver()
//{
//	player->GameOverManager();
//}