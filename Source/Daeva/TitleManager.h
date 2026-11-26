// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TitleManager.generated.h"

UCLASS()
class DAEVA_API ATitleManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATitleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "TitleUI")
	TSubclassOf<class UUserWidget> titleWedget;
	class UUserWidget* titleUI;
	UFUNCTION()
	void OnClickStartButton();
	UFUNCTION()
	void OnClickQuitButton();
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
