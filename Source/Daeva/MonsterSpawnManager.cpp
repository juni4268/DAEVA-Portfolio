// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnManager.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonsterSpawnManager::AMonsterSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterSpawnManager::BeginPlay()
{
	Super::BeginPlay();



	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (AActor* spawnPoint : allActors)
	{
		if (spawnPoint->GetName().Contains(TEXT("BP_MonsterSpawnPoint")))
		{
			monsterSpawnPoints.Add(spawnPoint);
		}
	}
	float spawnTime = FMath::RandRange(minSpawnTime, maxSpawnTime);
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle, this, &AMonsterSpawnManager::SpawnMonster, spawnTime);
}

// Called every frame
void AMonsterSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
}

void AMonsterSpawnManager::SpawnMonster()
{
	if (monsterCollection.IsEmpty() || monsterSpawnPoints.IsEmpty())
		return;


	float time = UGameplayStatics::GetRealTimeSeconds(GetWorld());

	if (time >= 150.f)
	{
		minSpawnTime = 1.5f;
		maxSpawnTime = 2.5f;
	}
	else if (time >= 120.f)
	{
		minSpawnTime = 2.0f;
		maxSpawnTime = 2.5f;
	}
	else if (time >= 60.f)
	{
		minSpawnTime = 2.0f;
		maxSpawnTime = 3.0f;
	}
	else if (time >= 1.f)
	{
		minSpawnTime = 2.5f;
		maxSpawnTime = 3.0f;
	}

	//int monCnt = FMath::RandRange(0, monsterCollection.Num() - 1);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	int spnCnt = FMath::RandRange(0, monsterSpawnPoints.Num() - 1);
	GetWorld()->SpawnActor<AMonster>(
		monsterCollection[0], monsterSpawnPoints[spnCnt]->GetActorLocation(), FRotator(0), Params);

	int currentMinute = time / 60;
	if (currentMinute > lastSpawnMinute)
	{
		int bossSpnCnt = FMath::RandRange(0, monsterSpawnPoints.Num() - 1);
		GetWorld()->SpawnActor<AMonster>(
			monsterCollection[1], monsterSpawnPoints[bossSpnCnt]->GetActorLocation(), FRotator(0), Params);
		lastSpawnMinute = currentMinute;
	}

	float spawnTime = FMath::RandRange(minSpawnTime, maxSpawnTime);
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle, this, &AMonsterSpawnManager::SpawnMonster, spawnTime);
}