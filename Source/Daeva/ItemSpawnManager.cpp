// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnManager.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AItemSpawnManager::AItemSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	float spawnTime = FMath::RandRange(minSpawnTime, maxSpawnTime);
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	for (AActor* spawnPoint : allActors) {
		if (spawnPoint->GetName().Contains(TEXT("BP_ItemSpawnPoint"))) {
			itemSpawnPoints.Add(spawnPoint);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle, this, &AItemSpawnManager::SpawnItem, spawnTime);
}

void AItemSpawnManager::SpawnItem()
{
	int monCnt = FMath::RandRange(0, itemCollection.Num() - 1);
	int spnCnt = FMath::RandRange(0, itemSpawnPoints.Num() - 1);
	GetWorld()->SpawnActor<AItem>(
		itemCollection[monCnt], itemSpawnPoints[spnCnt]->GetActorLocation(), FRotator(0));
	float spawnTime = FMath::RandRange(minSpawnTime, maxSpawnTime);
	GetWorld()->GetTimerManager().SetTimer(
		timerHandle, this, &AItemSpawnManager::SpawnItem, spawnTime);
}

// Called every frame
void AItemSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
