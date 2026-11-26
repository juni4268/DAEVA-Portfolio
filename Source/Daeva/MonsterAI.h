// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "Components/ActorComponent.h"
#include "MonsterAI.generated.h"

//열거형
UENUM(BlueprintType)
enum class MonsterState :uint8
{
	IdleState,
	MoveState,
	DamageState,
	DeathState,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAEVA_API UMonsterAI : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterAI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//행동패턴
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	MonsterState aiState = MonsterState::IdleState;
	void IdleState();
	void MoveState();
	void DamageState();
	void DeathState();

	//몬스터 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 hp = 100;
	UPROPERTY()
	class AArusa* player;
	UPROPERTY()
	class AMonster* monster;
	UPROPERTY()
	//몬스터 피격데미지
	class UMonsterAnim* anim;
	void OnMonsterDamage(int32 damageVal = 50);

	// AAiController 인스턴스의 포인터를 할당할 변수
	UPROPERTY()
	class AAIController* aiControl;

	//아이들 상태 대기시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float idleCoolTime = 1;
	// 공격반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float attackRange = 200.0f;
	// 공격후 대기시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float attackCoolTime = 2.f;
	// 피격후 대기 시간 1초
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float stunCoolTime = 0.1f;

	// DeltaTime을 기록할 변수
	float currentTime = attackCoolTime;
	// 몬스터 원더링 랜덤 위치를 생성하는 함수
	bool GetRandomLocation(FVector currentLocation, float range, FVector& location);
	// 랜덤 위치를 저장할 변수
	FVector randLocation;
	// 공격시 타겟이 이동한다면 몬스터의 공격방향을 타겟을 추적하도록
	void UpdateRotation(float DeltaTime);
	// 타겟을 기준으로 이동 패스를 생성.
	bool CreateNaviPath(FVector curLocation, FVector destLocation);
	//공격
	void Attack();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool IsBoss = false;
};
