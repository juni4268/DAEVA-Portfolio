// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USkillWidget::HideSkillCoolUI()
{
    if (Skill)
        Skill->SetVisibility(ESlateVisibility::Visible);

    if (NonSkill)
        NonSkill->SetVisibility(ESlateVisibility::Hidden);

    if (Skill_CoolTime)
        Skill_CoolTime->SetVisibility(ESlateVisibility::Hidden);
}

void USkillWidget::ShowSkillCoolUI(float TimeRemaining)
{
    if (Skill)
        Skill->SetVisibility(ESlateVisibility::Hidden);

    if (NonSkill)
        NonSkill->SetVisibility(ESlateVisibility::Visible);

    if (Skill_CoolTime)
        Skill_CoolTime->SetVisibility(ESlateVisibility::Visible);
        Skill_CoolTime->SetText(FText::AsNumber(TimeRemaining));
}
