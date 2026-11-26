// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Arusa.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetSphereRadius(80);
	RootComponent = collisionComp;
	particleComp =
		CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	particleComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemOverlap);
	particleComp->SetTemplate(spawnEffect);
	particleComp->SetWorldLocation(GetActorLocation() + FVector(0, 0, 0));
	particleComp->ActivateSystem();
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(
		deathTimer, this, &AItem::Die, lifeTime, false);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (itemMeshComp)
	{
		// 초당 180도 회전 (2초에 한 바퀴)
		float RotationRate = 180.0f; // Degrees per second
		// 이번 프레임에서의 회전 각도
		float DeltaRotation = RotationRate * DeltaTime;
		// 현재 회전 상태에 더하기
		FRotator NewRotation = itemMeshComp->GetComponentRotation();
		NewRotation.Yaw += DeltaRotation; // Z축 회전은 Yaw 값 변경
		// 새 회전 적용
		itemMeshComp->SetWorldRotation(NewRotation);
	}
}

void AItem::Die()
{
	particleComp->DeactivateSystem();
	Destroy();
}
void AItem::OnItemOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<AArusa>(OtherActor);
	if (player != nullptr)
	{
		OnCallbackOverlapActor(player);
	}
}
void AItem::OnCallbackOverlapActor(AArusa* player)
{
	Die();
}
