// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "ItemPotion.generated.h"



class AArusa;

UCLASS()
class DAEVA_API AItemPotion : public AItem
{

	GENERATED_BODY()

public:
	AItemPotion();
	void OnCallbackOverlapActor(AArusa* player) override;


	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioComp;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	USoundBase* ItempotionSound;

};
