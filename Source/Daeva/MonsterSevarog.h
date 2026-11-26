// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "MonsterSevarog.generated.h"


/**
 * 
 */
UCLASS()
class DAEVA_API AMonsterSevarog : public AMonster
{
	GENERATED_BODY()
public:
	AMonsterSevarog();
	void BeginPlay() override;

};
