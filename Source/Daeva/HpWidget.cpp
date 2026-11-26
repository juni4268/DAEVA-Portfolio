// Fill out your copyright notice in the Description page of Project Settings.


#include "HpWidget.h"
#include "Components/ProgressBar.h"



void UHpWidget::UpdateHPProgressBar(float Current, float Max)
{
	HP_Prog->SetPercent(Current / Max);
}
