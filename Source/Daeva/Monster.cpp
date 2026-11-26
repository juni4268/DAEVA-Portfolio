// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterAI.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MonsterAnim.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ai = CreateDefaultSubobject< UMonsterAI>(TEXT("AI"));
	//MonsterAIComponent = ai;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	SpawnEffect();
	SetCanBeDamaged(true);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::SpawnEffect()
{
	
	FTransform spawnEffectTrans;
	spawnEffectTrans.SetLocation(GetActorLocation() + FVector(0, 0, -80));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), spawnEffect, spawnEffectTrans);
}

void AMonster::DeathEffect()
{
	FTransform dieEffectTrans;
	dieEffectTrans.SetLocation(GetActorLocation() + FVector(0, 0, -80));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), deathEffect, dieEffectTrans);
}

//float AMonster::TakeDamage(
//	float DamageAmount,
//	FDamageEvent const& DamageEvent,
//	AController* EventInstigator,
//	AActor* DamageCauser)
//{
//	MonsterAIComponent->OnMonsterDamage(DamageAmount);
//	return DamageAmount;
//}