// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointWidget.generated.h"

UCLASS()
class DAEVA_API UKillWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 킬 수 갱신 함수
    void UpdateKillCount(int32 NewKillCount);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Point;
};

