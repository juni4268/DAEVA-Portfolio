// Fill out your copyright notice in the Description page of Project Settings.


#include "GamemanagerWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UGamemanagerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	if (Restart)
	{
		Restart->OnClicked.AddDynamic(this, &UGamemanagerWidget::OnRestart);
	}
	if (GameOut)
	{
		GameOut->OnClicked.AddDynamic(this, &UGamemanagerWidget::OnGameOut);
	}
}


void UGamemanagerWidget::GameOverOn()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetPause(true);
		// 마우스 커서 보이게
		PC->bShowMouseCursor = true;

		// 입력 모드를 UI 전용으로 설정
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());  // 현재 위젯에 포커스
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}

	GameOver->SetVisibility(ESlateVisibility::Visible);
	GameClear->SetVisibility(ESlateVisibility::Hidden);
	GameManager_Back->SetVisibility(ESlateVisibility::Visible);
	Restart->SetVisibility(ESlateVisibility::Visible);
	GameOut->SetVisibility(ESlateVisibility::Visible);
	EndPoint->SetVisibility(ESlateVisibility::Visible);
}

void UGamemanagerWidget::GameClearOn()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetPause(true);
		// 마우스 커서 보이게
		PC->bShowMouseCursor = true;

		// 입력 모드를 UI 전용으로 설정
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());  // 현재 위젯에 포커스
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}

	GameOver->SetVisibility(ESlateVisibility::Hidden);
	GameClear->SetVisibility(ESlateVisibility::Visible);
	GameManager_Back->SetVisibility(ESlateVisibility::Visible);
	Restart->SetVisibility(ESlateVisibility::Visible);
	GameOut->SetVisibility(ESlateVisibility::Visible);
	EndPoint->SetVisibility(ESlateVisibility::Visible);
}

void UGamemanagerWidget::GameManagerOff()
{
	GameOver->SetVisibility(ESlateVisibility::Hidden);
	GameClear->SetVisibility(ESlateVisibility::Hidden);
	GameManager_Back->SetVisibility(ESlateVisibility::Hidden);
	Restart->SetVisibility(ESlateVisibility::Hidden);
	GameOut->SetVisibility(ESlateVisibility::Hidden);
	EndPoint->SetVisibility(ESlateVisibility::Hidden);
}
void UGamemanagerWidget::OnRestart()
{
	FName levelName = FName(TEXT("DesertMap"));
	UGameplayStatics::OpenLevel(this, levelName);
}
void UGamemanagerWidget::OnGameOut()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}

void UGamemanagerWidget::EndKillCount(int32 NewKillCount)
{
	if (EndPoint)
	{
		FString EndCount = FString::Printf(TEXT("%d Point"), NewKillCount);
		EndPoint->SetText(FText::FromString(EndCount));
	}
}
