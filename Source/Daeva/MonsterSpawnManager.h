// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnManager.generated.h"

UCLASS()
class DAEVA_API AMonsterSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//스폰 시간
	UPROPERTY(EditDefaultsOnly, Category = "Property")
	float minSpawnTime= 3.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Property")
	float maxSpawnTime= 4.4f;
	TArray<class AActor*> monsterSpawnPoints;
	UPROPERTY(EditAnywhere, Category = "Property")
	TArray<TSubclassOf<class AMonster>> monsterCollection;
	FTimerHandle timerHandle;
	void SpawnMonster();

	int lastSpawnMinute = -1; // 최초 비교를 위해 -1로 시작

};
