// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"


class UTextBlock;
class UImage;

UCLASS()
class DAEVA_API USkillWidget : public UUserWidget
{
	
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ShowSkillCoolUI(float TimeRemaining);

	UFUNCTION(BlueprintCallable)
	void HideSkillCoolUI();
protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Skill;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Skill_CoolTime;

	UPROPERTY(meta = (BindWidget))
	UImage* NonSkill;
};
