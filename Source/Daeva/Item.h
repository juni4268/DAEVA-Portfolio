// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class AArusa;

UCLASS()
class DAEVA_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* collisionComp;
	UPROPERTY(VisibleAnywhere, Category = "BodyMesh")
	class UStaticMeshComponent* itemMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
	class UParticleSystemComponent* particleComp;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UParticleSystem* spawnEffect;
	UPROPERTY(EditDefaultsOnly, Category = "ItemProperty")
	float lifeTime = 15.0f;
	void Die();
	UFUNCTION()
	void OnItemOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnCallbackOverlapActor(AArusa* player);

};
