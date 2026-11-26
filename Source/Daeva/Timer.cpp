// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"
#include "Components/TextBlock.h"
#include "Arusa.h"
#include "Kismet/GameplayStatics.h"

	void UTimer::NativeConstruct()
	{
		Super::NativeConstruct();
		auto actor = UGameplayStatics::GetActorOfClass(
			GetWorld(), AArusa::StaticClass());
		player = Cast<AArusa>(actor);
		IsTimerEnd = false;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UTimer::UpdateTimer,
			1.0f,
			true
		);
	}
	void UTimer::UpdateTimer()
	{
		if (Maxtime <= 0.0f)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			IsTimerEnd = true;
		
			if (Timetext)
			{
				Timetext->SetText(FText::FromString(TEXT("00:00")));
			}

			OnTimerEnd.Broadcast();
			return;
		}

		Maxtime -= 1.0f;
		int32 Minutes = FMath::FloorToInt(Maxtime / 60);
		int32 Seconds = FMath::FloorToInt(FMath::Fmod(Maxtime, 60));
		FString TimeSring = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		if (Timetext)
		{
			Timetext->SetText(FText::FromString(TimeSring));
		}
	
	}
