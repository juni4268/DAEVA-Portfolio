// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI.h"
#include "Monster.h"
#include "MonsterAnim.h"
#include "Arusa.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PointWidget.h"

// Sets default values for this component's properties
UMonsterAI::UMonsterAI()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//if (anim)
	//{
	//	anim->PlaySpawnMontage();
	//	UE_LOG(LogTemp, Warning, TEXT("작동"));
	//}
	// ...
}


// Called when the game starts
void UMonsterAI::BeginPlay()
{
		Super::BeginPlay();
		auto actor = UGameplayStatics::GetActorOfClass(
			GetWorld(), AArusa::StaticClass());
		player = Cast<AArusa>(actor);
		monster = Cast<AMonster>(GetOwner());
		anim = Cast<UMonsterAnim>(monster->GetMesh()->GetAnimInstance());
		monster->SpawnDefaultController();
		aiControl = Cast<AAIController>(monster->GetController());
		anim->monsterAI = this;
		currentTime = attackCoolTime;
}


// Called every frame
void UMonsterAI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (aiState)
	{
	case MonsterState::IdleState:
		IdleState();
		break;
	case MonsterState::MoveState:
		MoveState();
		break;
	case MonsterState::DamageState:
		DamageState();
		break;
	case MonsterState::DeathState:
		DeathState();
		break;
	}
	currentTime += GetWorld()->DeltaTimeSeconds;
}

void UMonsterAI::IdleState() 
{
	//currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleCoolTime)
	{
		currentTime = 0.;
		aiState = MonsterState::MoveState;
		anim->aiState = aiState;
		GetRandomLocation(monster->GetActorLocation(), 500, randLocation);
	}
}
void UMonsterAI::MoveState()
{
	if (CreateNaviPath(monster->GetActorLocation(), player->GetActorLocation()))
	{
		aiControl->MoveToLocation(player->GetActorLocation());
	}
	else
	{
		auto result = aiControl->MoveToLocation(randLocation);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomLocation(monster->GetActorLocation(), 500, randLocation);
		}
	}
	FVector dir = player->GetActorLocation() - monster->GetActorLocation();
	if (dir.Size() < attackRange)
	{
		aiControl->StopMovement();
		anim->aiState = aiState;
		//aiState = MonsterState::IdleState;
		Attack();
	}
}

void UMonsterAI::Attack()
{
	/*currentTime += GetWorld()->DeltaTimeSeconds;*/
	if (currentTime > attackCoolTime)
	{
		currentTime = 0;
		anim->PlayAttackMontage();
	}
	UpdateRotation(GetWorld()->DeltaTimeSeconds);
	float distance = FVector::Distance(
		player->GetActorLocation(), monster->GetActorLocation());


	if (distance > attackRange)
	{
		aiState = MonsterState::MoveState;
		anim->aiState = aiState;
		GetRandomLocation(monster->GetActorLocation(), 100, randLocation);
	}
}
void UMonsterAI::DamageState()
{
	if (currentTime > stunCoolTime)
	{
		//aiState = MonsterState::IdleState;
		aiState = MonsterState::MoveState;
		currentTime = 0;
		anim->aiState = aiState;
	}
}
void UMonsterAI::DeathState()
{
	if (anim->isDeath == false)
	{
		return;
	}
	//몬스터 사망후 추락
	FVector p0 = monster->GetActorLocation();
	FVector vt = FVector::DownVector * 300.0f * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	monster->SetActorLocation(p);
	
	if (p.Z < -150.0f)
	{
		monster->Destroy();
	}
}


//몬스터 피격데미지
void UMonsterAI::OnMonsterDamage(int32 damageVal)
{
	hp -= damageVal;
	if (hp > 0)
	{
		aiState = MonsterState::DamageState;
		int32 idx = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("HitReact%d"), idx);
		anim->PlayDamageMontage(FName(*sectionName));
	}
	else
	{
		if (aiState != MonsterState::DeathState)
		{
			aiState = MonsterState::DeathState;
			monster->GetCapsuleComponent()->SetCollisionEnabled(
				ECollisionEnabled::NoCollision);
			anim->PlayDeathMontage(TEXT("DeathStart"));
			monster->DeathEffect();
			if (IsBoss)
			{
				player->KillPoint += 50;
				player->UpdatePoint();
			}
			else
			{
				player->KillPoint += 10;
				player->UpdatePoint();
			}
		}
	}
	anim->aiState = aiState;
}
//내비게이션
bool UMonsterAI::CreateNaviPath(FVector curLocation, FVector destLocation)
{
	FVector dir = destLocation - curLocation;
	UNavigationSystemV1* pns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destLocation);
	aiControl->BuildPathfindingQuery(req, query);
	FPathFindingResult r = pns->FindPathSync(query);
	return r.Result == ENavigationQueryResult::Success ? true : false;
}
//대기상태 움직임
bool UMonsterAI::GetRandomLocation(FVector currentLocation, float range, FVector& location)
{
	UNavigationSystemV1* p = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation nvloc;
	bool result = p->GetRandomReachablePointInRadius(currentLocation, range, nvloc);
	location = nvloc.Location;
	return result;
}
//몬스터 회전
void UMonsterAI::UpdateRotation(float DeltaTime)
{
	// 현재 위치에서 타겟 위치까지의 방향을 계산
	FVector DirectionToTarget =
		(player->GetActorLocation() - monster->GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = DirectionToTarget.Rotation();
	// 현재 컨트롤러의 회전
	FRotator CurrentRotation = aiControl->GetControlRotation();
	// 회전 보간 (현재 회전에서 목표 회전까지 부드럽게 회전)
	float InterpSpeed = 10.0f; // 회전 보간 속도
	FRotator NewRotation =
		FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);
	// 새로운 회전 적용
	monster->SetActorRotation(NewRotation);


}

