// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

// Sets default values
ATitleManager::ATitleManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATitleManager::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		FString levelName = GetWorld()->GetMapName();
		levelName = levelName.Replace(TEXT("UEDPIE_0_"), TEXT(""));
		if (levelName == TEXT("TitleMap"))
		{
			playerController->bShowMouseCursor = true;
		}
	}
	titleUI = CreateWidget(GetWorld(), titleWedget);
	if (titleUI != nullptr)
	{
		titleUI->AddToViewport();
		UButton* startBtn =
			Cast<UButton>(titleUI->GetWidgetFromName(TEXT("StartButton")));
		if (startBtn != nullptr)
		{
			startBtn->OnClicked.AddDynamic(this, &ATitleManager::OnClickStartButton);
		}
		UButton* quitBtn = Cast<UButton>(titleUI->GetWidgetFromName(TEXT("QuitButton")));
		if (quitBtn != nullptr)
		{
			quitBtn->OnClicked.AddDynamic(this, &ATitleManager::OnClickQuitButton);
		}
	}
}

// Called every frame
void ATitleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATitleManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATitleManager::OnClickStartButton()
{
	FName levelName = FName(TEXT("DesertMap"));
	UGameplayStatics::OpenLevel(this, levelName);
}
void ATitleManager::OnClickQuitButton()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		UKismetSystemLibrary::QuitGame(
			this, playerController, EQuitPreference::Quit, false);
	}
}
void ATitleManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (titleUI)
	{
		titleUI->RemoveFromViewport();
	}
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		playerController->bShowMouseCursor = false;
	}
}
