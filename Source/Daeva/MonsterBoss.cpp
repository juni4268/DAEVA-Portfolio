// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBoss.h"
#include "MonsterAnim.h"



AMonsterBoss::AMonsterBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(
			FVector(0, 0, -88), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));


	}

}
void AMonsterBoss::BeginPlay()
{
	Super::BeginPlay();

	
}
