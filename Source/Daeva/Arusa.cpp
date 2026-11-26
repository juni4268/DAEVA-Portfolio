// Fill out your copyright notice in the Description page of Project Settings.


#include "Arusa.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "PlayerAnimInstance.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Arrow.h"
#include "Monster.h"
#include "MonsterAI.h"
#include "Components/Button.h"
#include "SprintWidget.h"
#include "Timer.h"
#include "PotionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HpWidget.h"
#include "Components/TextBlock.h"
#include "PointWidget.h"
#include "GamemanagerWidget.h"
#include "Components/AudioComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "SkillWidget.h"

//UE_LOG(LogTemp, Warning, TEXT(""));
// Sets default values
AArusa::AArusa()
{
	PrimaryActorTick.bCanEverTick = true;
	//캐릭터 매쉬
	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/BattleWizardPolyart/Meshes/WizardSM.WizardSM'"));
		if (PlayerMesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
			GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		}
	//카메라
		if (springArmComponent == nullptr)
		{
			springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
			springArmComponent->SetupAttachment(RootComponent);
			springArmComponent->SetRelativeLocation(FVector(0, 40, 160));
			springArmComponent->TargetArmLength = springArmTargetLength;
			springArmComponent->bUsePawnControlRotation = true;
		}
		if (cameraComponent == nullptr)
		{
			cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
			cameraComponent->SetupAttachment(springArmComponent);
			cameraComponent->bUsePawnControlRotation = false;
		}
		//방향설정시 캐릭회전
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
		GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
		GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
		//무기
		WandMesh = CreateDefaultSubobject< UStaticMeshComponent >(TEXT("WandComp"));
		WandMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
		ConstructorHelpers::FObjectFinder<UStaticMesh> tempWandMesh(TEXT("/Script/Engine.StaticMesh'/Game/BattleWizardPolyart/Meshes/MagicStaffs/Staff01SM.Staff01SM'"));
		if (tempWandMesh.Succeeded())
		{
			WandMesh->SetStaticMesh(tempWandMesh.Object);
			WandMesh->SetRelativeLocation(FVector(-7, 7, -4));
			WandMesh->SetRelativeRotation(FRotator(0, -180, 0));
			WandMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		}
		ConstructorHelpers::FObjectFinder<USoundBase> castSoundtemp(TEXT("/Script/Engine.SoundWave'/Game/Sound/cast.cast'"));
		if (castSoundtemp.Succeeded()) {
			castsound = castSoundtemp.Object;
		}
		ConstructorHelpers::FObjectFinder<USoundBase> hitSoundtemp(TEXT("/Script/Engine.SoundWave'/Game/Sound/mixkit-wood-hard-hit-2182.mixkit-wood-hard-hit-2182'"));
		if (hitSoundtemp.Succeeded()) {
			hitsound = hitSoundtemp.Object;
		}
		ConstructorHelpers::FObjectFinder<USoundBase> tpSoundtemp(TEXT("/Script/Engine.SoundWave'/Game/Sound/TP_sound.TP_sound'"));
		if (tpSoundtemp.Succeeded()) {
			tpsound = tpSoundtemp.Object;
		}
		ConstructorHelpers::FObjectFinder<USoundBase> skillstart(TEXT("/Script/Engine.SoundWave'/Game/Sound/MagicStart.MagicStart'"));
		if (skillstart.Succeeded()) {
			skillstartsound = skillstart.Object;
		}
		ConstructorHelpers::FObjectFinder<USoundBase> skillattack(TEXT("/Script/Engine.SoundWave'/Game/Sound/MagicAttack.MagicAttack'"));
		if (skillattack.Succeeded()) {
			skillattacksound = skillattack.Object;
		}
		ConstructorHelpers::FObjectFinder<USoundBase> healtemp(TEXT("/Script/Engine.SoundWave'/Game/Sound/heal_use.heal_use'"));
		if (healtemp.Succeeded()) {
			healsound = healtemp.Object;
		}

}

// Called when the game starts or when spawned
void AArusa::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast< APlayerController >(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem*
			Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem >(
				PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(inputMappingContext, 0);
		}
		if (IsValid(PlayerController))
		{
			//내려보는 카메라 설정
			PlayerController->SetControlRotation(FRotator(-15.f, 0.f, 0.f));
			// 마우스 커서 비활성화
			PlayerController->bShowMouseCursor = false;

			// 입력 모드를 게임 전용으로 설정	
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetPause(false);
		}
	}

	//조준선UI생성
	crosshairUI = CreateWidget(GetWorld(), crosshairWidget);
	if (crosshairUI)
	{
		crosshairUI->AddToViewport();
	}
	//스프린트 쿨타임UI 생성
	SprintUI = CreateWidget(GetWorld(), SprintWidget);
	if (SprintUI)
	{
		SprintUI->AddToViewport();

		USprintWidget* SprintWidgetCasted = Cast<USprintWidget>(SprintUI);
		if (SprintWidgetCasted)
		{
			SprintWidgetCasted->HideCooltimeUI(); // 아이콘만 보여주기
		}
	}
	InitPlayer();
	//포션 UI생성
	PotionUI = CreateWidget(GetWorld(), PotionWidget);
	if (PotionUI)
	{
		PotionUI->AddToViewport();
		
		UPotionWidget* StartPotion = Cast<UPotionWidget>(PotionUI);
		StartPotion->OffPotionUI(PotionAmount);
		StartPotion->StartPotionUI(PotionAmount);
	}
	HpUI = CreateWidget(GetWorld(), HpWidget);
	if (HpUI)
	{
		HpUI->AddToViewport();
		UpdateHp();
	}
	GameManagerUI = CreateWidget<UGamemanagerWidget>(GetWorld(), GameManagerWidget);
	if (GameManagerUI)
	{
		GameManagerUI->AddToViewport();

		GameManagerUI->SetVisibility(ESlateVisibility::Hidden);
	}

	PointUI = CreateWidget<UKillWidget>(GetWorld(), PointWidget);
	if (PointUI)
	{
		PointUI->AddToViewport();
		UpdatePoint();
	}
	GameClear = false;
	SkillUI = CreateWidget<USkillWidget>(GetWorld(), SkillWidget);
	if (SkillUI)
	{
		SkillUI->AddToViewport();
		SkillUI->HideSkillCoolUI();
	}

}


// Called every frame
void AArusa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bSprintCooltime && !GetWorld()->GetTimerManager().IsTimerActive(SprintUIHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			SprintUIHandle,
			this,
			&AArusa::SprintCooltimeBar,
			1.0f,
			true
		);
	}
	if (!bSkillCooltime && !GetWorld()->GetTimerManager().IsTimerActive(SkillUIHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			SkillUIHandle,
			this,
			&AArusa::SkillCooltimeBar,
			1.0f,
			true
		);
	}
}

// Called to bind functionality to input
void AArusa::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(moveAction,
			ETriggerEvent::Triggered, this, &AArusa::InputActionMove);
		EnhancedInputComponent->BindAction(lookAndTurnAction,
			ETriggerEvent::Triggered, this, &AArusa::InputActionLookAndTurn);
		EnhancedInputComponent->BindAction(jumpAction,
			ETriggerEvent::Started, this, &AArusa::InputActionJump);
		EnhancedInputComponent->BindAction(AttackAction,
			ETriggerEvent::Started, this, &AArusa::InputActionAttack);
		EnhancedInputComponent->BindAction(AttackAction, 
			ETriggerEvent::Completed, this, &AArusa::InputActionAttack);
		EnhancedInputComponent->BindAction(SprintAction,
			ETriggerEvent::Started, this, &AArusa::InputActionSprint);
		EnhancedInputComponent->BindAction(PotionAction,
			ETriggerEvent::Started, this, &AArusa::InputActionPotion);
		EnhancedInputComponent->BindAction(SkillAction,
			ETriggerEvent::Started, this, &AArusa::InputActionSkill);
	}

}
//이동
void AArusa::InputActionMove(const FInputActionValue& Value)
{
	if (isDeath == true)
		return;
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);

	}
}

//화면회전
void AArusa::InputActionLookAndTurn(const FInputActionValue& Value)
{
	if (isDeath == true)
		return;
	const FVector2D lookTurnVector = Value.Get<FVector2D>();
	AddControllerYawInput(lookTurnVector.X);
	AddControllerPitchInput(lookTurnVector.Y);
}
//점프
void AArusa::InputActionJump(const FInputActionValue& Value)
{
	if (isDeath == true)
		return;
	if (Value.Get<bool>())
	{
		Jump();
	}
}

//공격(화살 발사)
void AArusa::PerformAttack()
{

	FTransform FireTransform = WandMesh->GetSocketTransform(TEXT("WeaponSocket"));
	FVector StartLocation = FireTransform.GetLocation();
	FVector EndLocation = cameraComponent->GetComponentLocation() + cameraComponent->GetForwardVector() * 5000;
	//사운드
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(), castsound, StartLocation, 2.0f);
	//발사위치
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AArrow* Arrow = GetWorld()->SpawnActor<AArrow>(ArrlowClass, FireTransform, SpawnParams);
	if (Arrow)
	{
		Arrow->SetVelocity(StartLocation, EndLocation);
	}

}
//공격 (애니메이션, 쿨타임)
void AArusa::InputActionAttack(const FInputActionValue& Value)
{
	if (!bCanAttack || !Value.Get<bool>() || isDeath)
		return;
	else
	{
		//연속공격 금지
		bCanAttack = false;
		//공격 애니메이션
		OnAttackAnimation();
		GetWorld()->GetTimerManager().SetTimer(
			attackCooldownTimer,			//타이머함수
			this,							//대상
			&AArusa::ResetAttackCooldown,	//쿨타임 해제
			attackCooldownDuration,			//쿨타임 지속시간
			false							//반복 안함
		);

	}
}
//공격 쿨타임
void AArusa::ResetAttackCooldown()
{
	bCanAttack = true;
}

//죽음 애니메이션 끝
void AArusa::DeathEnd()
{

}
//공격 애니메이션 코드
void AArusa::OnAttackAnimation()
{
	auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->PlayerAttackMontage();
}
//데미지 애니메이션 코드
void AArusa::OnHitAnimation()
{
	auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->PlayerDamageMontage();
}
//죽음 애니메이션 코드
void AArusa::OnDeathAnimation()
{
	//GameOverManager();
	auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	anim->PlayerDeathMontage(TEXT("DeathStart"));

}

//피격데미지
void AArusa::OnDamage()
{
	if (isDeath == true)
		return;
	currentHP -= 50;
	FTransform FireTransform = WandMesh->GetSocketTransform(TEXT("WeaponSocket"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitsound, FireTransform.GetLocation(), 2.f);;
	UpdateHp();
	OnHitAnimation();
	if (0 >= currentHP)
	{
		isDeath = true;
		//EndPoint();
		OnDeathAnimation();
		GameOverManager();
	}
}
void AArusa::OnBossDamage()
{
	if (isDeath == true)
		return;
	currentHP -= 100;
	FTransform FireTransform = WandMesh->GetSocketTransform(TEXT("WeaponSocket"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitsound, FireTransform.GetLocation(), 2.f);;
	UpdateHp();
	OnHitAnimation();
	if (0 >= currentHP)
	{
		isDeath = true;
		//EndPoint();
		OnDeathAnimation();
		GameOverManager();
	}
}
//기본상태설정
void AArusa::InitPlayer()
{
	currentHP = maxHP;
	isDeath = false;
	UpdateHp();
}

//텔레포트 스킬
void AArusa::InputActionSprint(const FInputActionValue& Value)
{
	if (!bSprintCooltime)
		return;
	bSprintCooltime = false;
	SprintCurrentTime = SprintCooltimeDuration;
	const FVector Forward = GetActorForwardVector();
	const FVector Up = GetActorUpVector();
	const FVector StartLocation = GetActorLocation();
	FVector Destination = StartLocation + Forward * 800.f;

	//  1. 출발 이펙트 (현재 위치)
	if (TPEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TPEffect,
			StartLocation,
			GetActorRotation(),
			FVector(1.f),
			true, true, ENCPoolMethod::None, true
		);
	}

	//  2. 텔레포트 시도
	bool bTeleported = TeleportTo(Destination, GetActorRotation());

	if (!bTeleported)
	{
		// 실패 시 살짝 위로 이동해서 재시도
		Destination += Up * 150.f;
		bTeleported = TeleportTo(Destination, GetActorRotation());
	}

	if (bTeleported)
	{
		//  3. 도착 이펙트 (성공한 위치)
		if (TPEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				TPEffect,
				Destination,
				GetActorRotation(),
				FVector(1.f),
				true, true, ENCPoolMethod::None, true
			);
		}

		//  4. 사운드 (도착 지점 기준)
		if (tpsound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), tpsound, Destination);
		}
		//  5. 쿨타임 타이머 시작
		GetWorld()->GetTimerManager().SetTimer(
			SprintCoolTimer,
			this,
			&AArusa::ResetSprintCooltime,
			SprintCooltimeDuration,
			false
		);
	}

}
//지속시간후 감속
//void AArusa::StopSprint()
//{
//	bIsSprinting = false;
//	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
//}
void AArusa::ResetSprintCooltime()
{
	bSprintCooltime = true;
}

//스프린트 쿨타임 타이머에서 호출
void AArusa::SprintCooltimeBar()
{
	SprintCurrentTime -= 1.f;

	if (SprintUI)
	{
		if(USprintWidget* Widget = Cast<USprintWidget>(SprintUI))
		{
			if (Widget->IsValidLowLevel())
			{
				Widget->ShowCooltimeUI(SprintCurrentTime);
			}
		}
	}
	if (SprintCurrentTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SprintUIHandle);
		if (SprintUI)
		{
			if(USprintWidget* Widget = Cast<USprintWidget>(SprintUI))
			{
				Widget->HideCooltimeUI();
			}
		}
	}
}
//포션
void AArusa::GetPotion()
{
	if (UPotionWidget* Widget = Cast<UPotionWidget>(PotionUI))
	{

		PotionAmount += 1;
		Widget->OnPotionUI(PotionAmount);
	}
}
void AArusa::OnPotionAnimation()
{
	auto anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->PlayerPotionMontage();
}
void AArusa::InputActionPotion(const FInputActionValue& Value)
{

	if (!bPotioncool || isDeath || currentHP == maxHP)
		return;
	bPotioncool = false;
	OnPotionAnimation();
	GetWorld()->GetTimerManager().SetTimer(
		PotionCoolTimer,
		this,
		&AArusa::ResetPotion,
		1.5f,
		false
	);
	UPotionWidget* Widget = Cast<UPotionWidget>(PotionUI);
	if (PotionAmount > 1)
	{
		FVector playerLocation = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(
		GetWorld(), healsound, playerLocation, 1.f);
		PotionAmount -= 1;
		Widget->OnPotionUI(PotionAmount);
		OnHealEffect();
		currentHP += 50;
		UpdateHp();
	}
	else if (PotionAmount == 1)
	{
		PotionAmount -= 1;
		Widget->OffPotionUI(PotionAmount);
		OnHealEffect();
		currentHP += 50;
		UpdateHp();
	}
	else
	{
		Widget->OffPotionUI(PotionAmount);
		PotionAmount = 0;
	}
}
void AArusa::ResetPotion()
{
	bPotioncool = true;
}

void AArusa::OnHealEffect()
{
	if (HealEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(
			HealEffect,
			GetMesh(),                         // 메시 기준
			HealEffectSocketName,              // 소켓 이름 (없으면 NAME_None)
			FVector::ZeroVector,               // 오프셋
			FRotator::ZeroRotator,             // 회전
			EAttachLocation::SnapToTarget,     // 위치/회전 일치
			true							   // 자동 파괴
		);
		
	}
}              

void AArusa::UpdateHp()
{
	if (!HpUI) return;
	UHpWidget* HpWidgetCasted = Cast<UHpWidget>(HpUI);
	if (HpWidgetCasted)
	{
		HpWidgetCasted->UpdateHPProgressBar(currentHP, maxHP);
	}

}

void AArusa::ChangeTitleLevel()
{
	FName levelName = FName(TEXT("TitleMap"));
	UGameplayStatics::OpenLevel(this, levelName);
}

void AArusa::UpdatePoint()
{	
	if (!PointUI) return;
	UKillWidget* PointWidgetCasted = Cast<UKillWidget>(PointUI);
	if (PointWidgetCasted)
	{
		PointWidgetCasted->UpdateKillCount(KillPoint);
	}

}

void AArusa::GameOverManager()
{
	GameManagerUI->GameOverOn();
	GameManagerUI->SetVisibility(ESlateVisibility::Visible);
	GameManagerUI->EndKillCount(KillPoint);
}
void AArusa::GameClearManager()
{
	KillPoint += 50;
	GameManagerUI->GameClearOn();
	GameManagerUI->SetVisibility(ESlateVisibility::Visible);
	GameManagerUI->EndKillCount(KillPoint);
}


//스킬
//void AArusa::InputActionSkill()
//{
//	if (!bSkillCooltime)
//		return;
//	bSkillCooltime = false;
//	SkillCurrentTime = SkillCooltimeDuration;
//	GetWorld()->GetTimerManager().SetTimer(
//		SkillCoolTimer,
//		this,
//		&AArusa::ResetSkillCooltime,
//		SkillCooltimeDuration,
//		false
//	);
//	FVector SkillLocation = GetActorLocation();
//	const FVector Down = GetActorUpVector();
//	// 1단계: 마법진 생성 (즉시)
//	UGameplayStatics::PlaySoundAtLocation(
//		GetWorld(), skillstartsound, SkillLocation, 0.5f);
//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//		GetWorld(),
//		MagicCircleEffect,
//		SkillLocation + Down * -50.f,
//		FRotator::ZeroRotator,
//		FVector(15.f, 15.f, 1.f),
//		true,
//		true,
//		ENCPoolMethod::AutoRelease,
//		true
//	);
//
//	// 2단계: 2초 후 폭발 이펙트 + 데미지
//	GetWorld()->GetTimerManager().SetTimer(SkillExplosionDelayHandle, FTimerDelegate::CreateLambda([this, SkillLocation]()
//		{
//			// 폭발 이펙트
//			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//				GetWorld(),
//				ExplosionEffect,
//				SkillLocation,
//				FRotator::ZeroRotator,
//				FVector(1.f),
//				true,
//				true,
//				ENCPoolMethod::AutoRelease,
//				true
//			);
//
//			// 폭발 데미지 적용
//			TArray<AActor*> IgnoredActors;
//			TArray<FHitResult> HitResults;
//			bool bHit = GetWorld()->SweepMultiByChannel(
//				HitResults,
//				SkillLocation,
//				SkillLocation,
//				FQuat::Identity,
//				ECC_Pawn,  // Pawn 충돌 채널로 탐지
//				FCollisionShape::MakeSphere(ExplosionRadius)
//			);
//			for (auto& Hit : HitResults)
//			{
//
//				AActor* HitActor = Hit.GetActor();
//				if (!HitActor || HitActor == this) continue;
//				UGameplayStatics::ApplyRadialDamage(
//					GetWorld(),
//					ExplosionDamage,
//					SkillLocation,
//					ExplosionRadius,
//					UDamageType::StaticClass(),
//					IgnoredActors,
//					this,
//					GetController(),
//					true
//				);
//				//히트이펙트
//				UGameplayStatics::PlaySoundAtLocation(
//					GetWorld(), skillattacksound, SkillLocation, 0.5f);
//				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//					GetWorld(),
//					SkillHitEffect,                    // NiagaraSystem*
//					HitActor->GetActorLocation(), // 또는 Hit.ImpactPoint
//					FRotator::ZeroRotator,
//					FVector(1.f),
//					true
//				);
//			}
//		}), 2.f, false);
//}
void AArusa::InputActionSkill()
{
	if (!bSkillCooltime)
		return;

	bSkillCooltime = false;
	SkillCurrentTime = SkillCooltimeDuration;

	GetWorld()->GetTimerManager().SetTimer(
		SkillCoolTimer,
		this,
		&AArusa::ResetSkillCooltime,
		SkillCooltimeDuration,
		false
	);

	FVector SkillLocation = GetActorLocation();
	const FVector Down = GetActorUpVector();

	// 1단계: 마법진 생성 (즉시)
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(), skillstartsound, SkillLocation, 0.5f);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		MagicCircleEffect,
		SkillLocation + Down * -50.f,
		FRotator::ZeroRotator,
		FVector(15.f, 15.f, 1.f),
		true,
		true,
		ENCPoolMethod::AutoRelease,
		true
	);

	// 2단계: 2초 후 폭발 이펙트 + 데미지
	GetWorld()->GetTimerManager().SetTimer(SkillExplosionDelayHandle, FTimerDelegate::CreateLambda([this, SkillLocation]()
		{
			// 폭발 이펙트
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplosionEffect,
				SkillLocation,
				FRotator::ZeroRotator,
				FVector(1.f),
				true,
				true,
				ENCPoolMethod::AutoRelease,
				true
			);

			// 스킬 범위 안의 몬스터 탐색
			TArray<FHitResult> HitResults;
			bool bHit = GetWorld()->SweepMultiByChannel(
				HitResults,
				SkillLocation,
				SkillLocation,
				FQuat::Identity,
				ECC_Pawn,
				FCollisionShape::MakeSphere(ExplosionRadius)
			);

			for (auto& Hit : HitResults)
			{
				AActor* HitActor = Hit.GetActor();
				if (!HitActor || HitActor == this) continue;

				// 몬스터인지 확인하고 데미지 함수 호출
				if (auto monster = Cast<AMonster>(HitActor))
				{
					auto ai = HitActor->GetDefaultSubobjectByName(TEXT("AI"));
					if (ai)
					{
						Cast<UMonsterAI>(ai)->OnMonsterDamage(ExplosionDamage);
					}

					// 히트 이펙트 + 사운드
					UGameplayStatics::PlaySoundAtLocation(
						GetWorld(), skillattacksound, SkillLocation, 0.5f);

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						SkillHitEffect,
						HitActor->GetActorLocation(),
						FRotator::ZeroRotator,
						FVector(1.f),
						true
					);
				}
			}
		}), 2.f, false);
}

void AArusa::SkillCooltimeBar()
{
	SkillCurrentTime -= 1.f;

	SkillUI->ShowSkillCoolUI(SkillCurrentTime);

	if (SkillCurrentTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillUIHandle);

		SkillUI->HideSkillCoolUI();

	}
}
void AArusa::ResetSkillCooltime()
{
	bSkillCooltime = true;
}
