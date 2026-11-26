// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


//쿨타임 아닐때
void USprintWidget::HideCooltimeUI()
{
    if (Sprint)
        Sprint->SetVisibility(ESlateVisibility::Visible);

    if (NonSprint)
        NonSprint->SetVisibility(ESlateVisibility::Hidden);

    if (Sprint_CoolTime)
        Sprint_CoolTime->SetVisibility(ESlateVisibility::Hidden);
}

//쿨타임 일때
void USprintWidget::ShowCooltimeUI(float TimeRemaining)
{
    if (Sprint)
        Sprint->SetVisibility(ESlateVisibility::Hidden);

    if (NonSprint)
        NonSprint->SetVisibility(ESlateVisibility::Visible);

    if (Sprint_CoolTime)
    {
        Sprint_CoolTime->SetVisibility(ESlateVisibility::Visible);
        Sprint_CoolTime->SetText(FText::AsNumber(TimeRemaining));
    }
}