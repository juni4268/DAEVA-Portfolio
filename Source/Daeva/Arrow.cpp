// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Arusa.h"
#include "Monster.h"
#include "MonsterAi.h"
#include "Engine/OverlapResult.h"


// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject< USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetSphereRadius(13);
	RootComponent = collisionComp;
	ArrowMeshComp = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("ArrowoMeshComp"));
	ArrowMeshComp->SetupAttachment(collisionComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MixedVFX/Meshes/Sircle.Sircle'"));
			if (TempMesh.Succeeded())
			{
				ArrowMeshComp->SetStaticMesh(TempMesh.Object);
				ArrowMeshComp->SetRelativeLocation(FVector(0, 0, 0));
				ArrowMeshComp->SetRelativeRotation(FRotator(0, 0, 0));
				ArrowMeshComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
				ArrowMeshComp->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
			}
	movementComp = CreateDefaultSubobject< UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	//투사체 속도
	movementComp->InitialSpeed = 6000;
	movementComp->MaxSpeed = 6000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;
	movementComp->ProjectileGravityScale = 0.0f;

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(
		TEXT("/Script/Engine.SoundWave'/Game/Sound/arrowhit.arrowhit'"));
	if (tempSound.Succeeded())
	{
		hitSound = tempSound.Object;
	}
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	//따라가는 이펙트?
	Super::BeginPlay();
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnArrowOverlap);
	FTimerHandle deathTimer;
	TWeakObjectPtr<AArrow> WeakArrow = this;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda(
		[WeakArrow]()
		{
			if (WeakArrow.IsValid())
			{
				WeakArrow->Die();
			}
		}
	), 1.f, false);
	
	if (LightningSystem && ArrowMeshComp)
{
    LightningEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
        LightningSystem,
        ArrowMeshComp,
        TEXT("ArrowSocket"),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        true,
        true
    );

    if (LightningEffect)
    {
        LightningEffect->Activate();
    }
}

}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AArrow::Die()
{
	if (LightningEffect)
	{
		LightningEffect->DestroyComponent();  // 이펙트 컴포넌트 수동 제거
		LightningEffect = nullptr;	
	}
	Destroy();
}
void AArrow::OnArrowOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		FTransform impactTrans;
		impactTrans.SetLocation(SweepResult.ImpactPoint);
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),hitSound, SweepResult.ImpactPoint, 0.5);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			impactEffect,                  // UNiagaraSystem* 타입
			SweepResult.ImpactPoint,
			FRotator::ZeroRotator,
			FVector(1.f)                         // Scale
		);
		//화살 몬스터 피격?
		auto monster = Cast<AMonster>(OtherActor);
		if (monster != nullptr) {
			auto ai = OtherActor->GetDefaultSubobjectByName(TEXT("AI"));
			//화살 데미지
			if (ai != nullptr) {
				Cast<UMonsterAI>(ai)->OnMonsterDamage(50);
			}
		}
		ApplyAreaDamage(SweepResult.ImpactPoint, OtherActor);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AreaEffect,                  // UNiagaraSystem* 타입
			SweepResult.ImpactPoint,
			FRotator::ZeroRotator,
			FVector(1.f)
		);
	}
	Die();
}
void AArrow::SetVelocity(FVector startPos, FVector targetPos)
{
	FVector direction = targetPos - startPos;
	direction.Normalize();
	movementComp->Velocity = direction * movementComp->InitialSpeed;
}


void AArrow::ApplyAreaDamage(const FVector& Center, AActor* IgnoredActor)
{
	const float SphereRadius = 200.f;
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (IgnoredActor)
	{
		Params.AddIgnoredActor(IgnoredActor); // 직접 충돌한 대상은 다시 제외
	}
	Params.bTraceComplex = false;
	Params.bReturnPhysicalMaterial = false;

	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn, // 또는 몬스터가 사용하는 채널
		Sphere,
		Params
	);


	if (bHit)
	{
		for (const FOverlapResult& Result : Overlaps)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (OverlappedActor)
			{
				AMonster* OverlappedMonster = Cast<AMonster>(OverlappedActor);
				if (OverlappedMonster)
				{
					if (auto ai = OverlappedMonster->GetDefaultSubobjectByName(TEXT("AI")))
					{
						Cast<UMonsterAI>(ai)->OnMonsterDamage(40);
					}
				}
			}
		}

	}
}
