// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "MonsterBoss.generated.h"

/**
 * 
 */
UCLASS()
class DAEVA_API AMonsterBoss : public AMonster
{
	GENERATED_BODY()
public:
	AMonsterBoss();
	void BeginPlay() override;


};
