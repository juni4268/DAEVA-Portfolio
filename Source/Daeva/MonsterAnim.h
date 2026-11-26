// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MonsterAI.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnim.generated.h"

/**
 * 
 */
UCLASS()
class DAEVA_API UMonsterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMonsterAI* monsterAI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	MonsterState aiState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	bool isAttack = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	bool isDeath = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "AnimEvent")
	void PlayDamageMontage(FName sectionName);
	UFUNCTION(BlueprintImplementableEvent, Category = "AnimEvent")
	void PlayDeathMontage(FName sectionName);
	UFUNCTION(BlueprintImplementableEvent, Category = "AnimEvent")
	void PlayAttackMontage();


	UFUNCTION(BlueprintImplementableEvent, Category = "AnimEvent")
	void PlaySpawnMontage();
};
