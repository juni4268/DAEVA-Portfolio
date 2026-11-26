// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Arusa.h"
#include "GameFramework/CharacterMovementComponent.h"



void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<AArusa>(ownerPawn);
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector rightVector = player->GetActorRightVector();
		// X값 속도(세로)
		hDirectionSpeed = FVector::DotProduct(rightVector, velocity);
		// Y값 속도(가로)
		FVector forwardVector = player->GetActorForwardVector();
		vDirectionSpeed = FVector::DotProduct(forwardVector, velocity);

		auto movement = player->GetCharacterMovement();
		isFalling = movement->IsFalling();
	}
}
//공격 (화살발사)
void UPlayerAnimInstance::AnimNotify_AttackStart()
{
	if (AArusa* Player = Cast<AArusa>(TryGetPawnOwner()))
	{
		Player->PerformAttack(); // 화살 발사 또는 타격 처리
	}
}