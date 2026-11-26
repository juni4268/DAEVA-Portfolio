// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPotion.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Arusa.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"


AItemPotion::AItemPotion()
{
	itemMeshComp = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("ItemMeshComp"));
	itemMeshComp->SetupAttachment(collisionComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> PotionMesh(TEXT("/Script/Engine.StaticMesh'/Game/Image/Potion/potion.potion'"));
	if (PotionMesh.Succeeded())
	{
		itemMeshComp->SetStaticMesh(PotionMesh.Object);
		itemMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		itemMeshComp->SetRelativeLocation(FVector(0, 0, 120));
		itemMeshComp->SetRelativeRotation(FRotator(25, 0, 0));
		itemMeshComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}
	ConstructorHelpers::FObjectFinder<USoundBase> PotionSoundtemp(TEXT("/Script/Engine.SoundWave'/Game/Sound/heal_aura.heal_aura'"));
	if (PotionSoundtemp.Succeeded()) {
		ItempotionSound = PotionSoundtemp.Object;
	}
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->SetSound(ItempotionSound);
	AudioComp->bAutoActivate = true;
}

void AItemPotion::OnCallbackOverlapActor(AArusa* player)
{
	Super::OnCallbackOverlapActor(player);
	player->GetPotion();
	AudioComp->Stop();
}