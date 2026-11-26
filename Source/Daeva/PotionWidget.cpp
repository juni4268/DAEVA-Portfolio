// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPotionWidget::StartPotionUI(int amount)
{

	Potion_amount->SetText(FText::AsNumber(0));

}

void UPotionWidget::OnPotionUI(int amount)
{
	Potion->SetVisibility(ESlateVisibility::Visible);
	Potion_amount->SetVisibility(ESlateVisibility::Visible);
	NonPotion->SetVisibility(ESlateVisibility::Hidden);
	Potion_amount->SetText(FText::AsNumber(amount));
}

void UPotionWidget::OffPotionUI(int amount)
{
	Potion->SetVisibility(ESlateVisibility::Hidden);
	Potion_amount->SetVisibility(ESlateVisibility::Hidden);
	NonPotion->SetVisibility(ESlateVisibility::Visible);
	Potion_amount->SetText(FText::AsNumber(amount));
}
