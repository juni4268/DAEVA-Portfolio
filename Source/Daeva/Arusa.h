// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Arusa.generated.h"




UCLASS()
class DAEVA_API AArusa : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AArusa();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 카메라
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* springArmComponent;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* cameraComponent;

	float springArmTargetLength = 400;

	//이동
	void InputActionMove(const struct FInputActionValue& Value);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* inputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* moveAction;
	
	//화면이동
	void InputActionLookAndTurn(const struct FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* lookAndTurnAction;
	//점프
	void InputActionJump(const struct FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* jumpAction;
	//무기매쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wand")
	UStaticMeshComponent* WandMesh;
	//공격
	void InputActionAttack(const struct FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* AttackAction;
	//공격쿨타임
	void ResetAttackCooldown();
	FTimerHandle attackCooldownTimer;
	bool bCanAttack = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float attackCooldownDuration = 0.8f;
	//공격(타이밍 : 화살발사)
	UFUNCTION()
	void PerformAttack();
	//조준선
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> crosshairWidget;
	class UUserWidget* crosshairUI;
	//플레이어 화살인식
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
	TSubclassOf<class AArrow> ArrlowClass;
	//죽음 애니메이션 끝
	UFUNCTION(BlueprintCallable, Category = "PlayerEvent")
	void DeathEnd();

	//공격 애니메이션 작동
	UFUNCTION()
	void OnAttackAnimation();
	//데미지 애니메이션 작동
	UFUNCTION()
	void OnHitAnimation();
	//죽음 애니메이션 작동
	UFUNCTION()
	void OnDeathAnimation();
	//피격 데미지
	UFUNCTION(BlueprintCallable, Category = "PlayerEvent")
	void OnDamage();
	UFUNCTION(BlueprintCallable, Category = "PlayerEvent")
	void OnBossDamage();
	//플레이어 피격
	void InitPlayer();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerProperty")
	int maxHP = 250;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerProperty")
	int currentHP = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerProperty")
	bool isDeath = false;

	//스프린트 쿨타임
	void SprintCooltimeBar();
	void ResetSprintCooltime();
	bool bSprintCooltime = true;
	FTimerHandle SprintCoolTimer;
	FTimerHandle SprintUIHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintCooltimeDuration = 20.f;
	float SprintCurrentTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> SprintWidget;
	class UUserWidget* SprintUI;

	//달리기
	//void StopSprint();
	void InputActionSprint(const FInputActionValue& Value);
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float WalkSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float SprintSpeed = 1200.f;
	//UPROPERTY(EditAnywhere, Category = "Sprint")
	//float SprintDuration = 2.0f;	//지속시간
	//FTimerHandle SprintTimer;
	//bool bIsSprinting = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	//포션
	UFUNCTION()
	void OnPotionAnimation();
	void GetPotion();
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PotionWidget;
	class UUserWidget* PotionUI;
	int PotionAmount = 0;

	void InputActionPotion(const struct FInputActionValue& Value);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* PotionAction;
	//포션 타이머
	bool bPotioncool = true;
	FTimerHandle PotionCoolTimer;
	void ResetPotion();
	//포션 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UParticleSystem* HealEffect;
	UPROPERTY(EditAnywhere, Category = "Effects")
	FName HealEffectSocketName = "spineSocket";
	void OnHealEffect();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HpWidget;
	class UUserWidget* HpUI;
	void UpdateHp();

	void ChangeTitleLevel();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGamemanagerWidget> GameManagerWidget;
	UGamemanagerWidget* GameManagerUI;

	void GameOverManager();
	void GameClearManager();
	bool GameClear = false;
	//bool GameOver = false;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKillWidget> PointWidget;

	UKillWidget* PointUI;

	int32 KillPoint = 0;
	void UpdatePoint();


	//사운드
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* castsound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* hitsound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* TPEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* tpsound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* skillstartsound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* skillattacksound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* healsound;
	//스킬
	// 마법진과 폭발 이펙트
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* MagicCircleEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* ExplosionEffect;

	// 딜레이 타이머
	FTimerHandle SkillExplosionDelayHandle;

	// 데미지 수치, 반경
	UPROPERTY(EditAnywhere, Category = "PlayerProperty")
	float ExplosionDamage = 1000.f;

	UPROPERTY(EditAnywhere, Category = "PlayerProperty")
	float ExplosionRadius = 1200.f;
	void InputActionSkill();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* SkillAction;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* SkillHitEffect;
	//스킬 쿨
	void SkillCooltimeBar();
	void ResetSkillCooltime();
	bool bSkillCooltime = true;
	FTimerHandle SkillCoolTimer;
	FTimerHandle SkillUIHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCooltimeDuration = 45.f;
	float SkillCurrentTime = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USkillWidget> SkillWidget;
	USkillWidget* SkillUI;


};
