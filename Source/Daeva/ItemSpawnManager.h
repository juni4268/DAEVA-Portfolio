// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnManager.generated.h"

UCLASS()
class DAEVA_API AItemSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Property")
	float minSpawnTime = 10;
	UPROPERTY(EditAnywhere, Category = "Property")
	float maxSpawnTime = 15;
	TArray<class AActor*> itemSpawnPoints;
	UPROPERTY(EditAnywhere, Category = "Property")
	TArray<TSubclassOf<class AItem>> itemCollection;
	FTimerHandle timerHandle;
	void SpawnItem();

};
