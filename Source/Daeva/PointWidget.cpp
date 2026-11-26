// Fill out your copyright notice in the Description page of Project Settings.

#include "PointWidget.h"
#include "Components/TextBlock.h"

void UKillWidget::UpdateKillCount(int32 NewKillCount)
{
    if (Point)
    {
        Point->SetText(FText::AsNumber(NewKillCount));
    }
}
