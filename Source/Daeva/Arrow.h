// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class DAEVA_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 따라붙은 실제 Niagara 컴포넌트
	UPROPERTY()
	class UNiagaraComponent* LightningEffect;

	//충돌 이펙트

	//이동
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* movementComp;
	//충돌
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class USphereComponent* collisionComp;
	//메쉬
	UPROPERTY(VisibleAnywhere, Category = "BodyMesh")
	class UStaticMeshComponent* ArrowMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
	class UNiagaraSystem* impactEffect;
	//이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
	class UNiagaraSystem* LightningSystem;
	/*UPROPERTY(EditAnywhere, Category = "GrenadeEffect")
	class UParticleSystem* impactEffect;*/
	void Die();
	UFUNCTION()
	//충돌2
	void OnArrowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	/*UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* bomSound;;*/
	//속도
	void SetVelocity(FVector startPos, FVector targetPos);


	UFUNCTION()
	void ApplyAreaDamage(const FVector& Center, AActor* IgnoredActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning")
	class UNiagaraSystem* AreaEffect;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* hitSound;;
};
