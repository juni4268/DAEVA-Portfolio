// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DAEVA_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeUpdateAnimation(float DeltaSeconds) override;
	//속도 조절
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMoveState")
	float hDirectionSpeed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMoveState")
	float vDirectionSpeed = 0;
	//플레이어의 낙하 상태를 저장 false라면 지면에 있는 상태.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerMoveState")
	bool isFalling = false;

	//공격 애니메이션 몽타쥬
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerMontage")
	void PlayerAttackMontage();
	//데미지 애니메이션 몽타쥬
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerMontage")
	void PlayerDamageMontage();
	//죽음 애니메이션 몽타쥬
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerMontage")
	void PlayerDeathMontage(FName sessionName);
	//공격타이밍 설정
	UFUNCTION()
	void AnimNotify_AttackStart();
	//포션 애니메이션
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerMontage")
	void PlayerPotionMontage();
};
