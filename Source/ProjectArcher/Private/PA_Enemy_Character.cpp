// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Character.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_EnemyStat.h"
#include "PA_Enemy_HPBar.h"
#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_DamageWidget.h"
#include "PA_Object_PlayerArrow.h"
#include "PA_Player_PlayerController.h"

#include "Components/WidgetComponent.h"				//위젯 컴포넌트
#include "Engine/DamageEvents.h"					//대미지 이벤트

// Sets default values
APA_Enemy_Character::APA_Enemy_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트 생성
	EnemyStat = CreateDefaultSubobject<UPA_Enemy_EnemyStat>(TEXT("ENEMYSTAT"));
	EnemyHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ENEMYHPWIDGET"));
	DamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DAMAGEWIDGET"));

	//컴포넌트 부착
	EnemyHPWidget->SetupAttachment(GetMesh());	//HP위젯
	DamageWidget->SetupAttachment(GetMesh());	//대미지 위젯

	//위젯
		//HP위젯
	EnemyHPWidget->SetWidgetSpace(EWidgetSpace::Screen);				//HP 위젯이 항상 스크린에 보이도록 설정
	static ConstructorHelpers::FClassFinder<UPA_Enemy_HPBar>UI_HUD(TEXT("/Game/Widget/UI/WBP_EnemyHPBar.WBP_EnemyHPBar_C"));
	if (UI_HUD.Succeeded())
	{
		EnemyHPWidget->SetWidgetClass(UI_HUD.Class);								//기본 클래스로 설정
		EnemyHPWidget->SetDrawSize(FVector2D(150.0f, 50.0f));						//위젯의 크기 설정
	}
	EnemyHPWidget->SetHiddenInGame(true);	//HPBar숨김
		//대미지 위젯
	DamageWidget->SetWidgetSpace(EWidgetSpace::Screen);					//대미지 위젯이 항상 스크린에 보이도록 설정
	static ConstructorHelpers::FClassFinder<UPA_Enemy_DamageWidget>DAMAGE_WIDGET(TEXT("/Game/Widget/Damage/WBP_DamageWidget.WBP_DamageWidget_C"));
	if (DAMAGE_WIDGET.Succeeded())
	{
		DamageWidget->SetWidgetClass(DAMAGE_WIDGET.Class);							//기본 클래스로 설정
		DamageWidget->SetDrawSize(FVector2D(256.0f, 75.0f));						//위젯의 크기 설정
	}

	//AI설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	//소환되거나 이미 존재하는 경우에 AI컨트롤러 빙의

	//AI컨트롤러
	AIControllerClass = APA_Enemy_Controller::StaticClass();				//AI컨트롤러 클래스 설정

	//애니메이션 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//캐릭터 상태 설정
	CharacterState = ECharacterState::PREINIT;

	//프로퍼티 설정
	DeadTimer = 5.0f;
}

// Called when the game starts or when spawned
void APA_Enemy_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APA_Enemy_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APA_Enemy_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float APA_Enemy_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	EnemyStat->SetDamage(DamageAmount);
	auto DamageWidgetObject = Cast<UPA_Enemy_DamageWidget>(DamageWidget->GetWidget());

	auto ArrowObject = Cast<APA_Object_PlayerArrow>(DamageCauser);
	//평타 화살인지 확인
	if (ArrowObject)
	{
		//치명타 확인
		if (ArrowObject->IsCritical)
		{
			DamageWidgetObject->PlayDamageAnimation(FinalDamage, true);
		}
		else
		{
			DamageWidgetObject->PlayDamageAnimation(FinalDamage, false);
		}
	}
	else
	{
		DamageWidgetObject->PlayDamageAnimation(FinalDamage, false);
	}

	//처치한 플레이어의 컨트롤러
	if (PlayerController != Cast<APA_Player_PlayerController>(EventInstigator))
	{
		PlayerController = Cast<APA_Player_PlayerController>(EventInstigator);
	}

	DamagedPlayerCharacter = EventInstigator->GetPawn();

	return FinalDamage;
}

void APA_Enemy_Character::SetEnemyState(ECharacterState NewState)
{
	CharacterState = NewState;

	switch (CharacterState)
	{
		case ECharacterState::LOADING:
		{
			SetActorHiddenInGame(true);				//액터 숨김
			EnemyHPWidget->SetHiddenInGame(true);	//HPBar숨김
			SetCanBeDamaged(false);					//대미지 입지 않음
		}
		break;

		case ECharacterState::READY:
		{
			EnemyAI->RunAI();		//AI컨트롤러의 BehaviorTree작동

			SetActorHiddenInGame(false);			//액터 드러냄
			//EnemyHPWidget->SetHiddenInGame(false);	//HPBar 드러냄
			SetCanBeDamaged(true);					//대미지 입음

			//HP가 제로되었을 때
			EnemyStat->OnHPIsZeroDelegate.AddLambda([this]()->void {SetEnemyState(ECharacterState::DEAD); });	//DEAD로 상태 변환

			auto EnemyWidget = Cast<UPA_Enemy_HPBar>(EnemyHPWidget->GetUserWidgetObject());
			if (EnemyWidget != nullptr) EnemyWidget->BindEnemyStat(EnemyStat);	//적 스텟 델리게이트를 바인드함
		}
		break;

		case ECharacterState::DEAD:
		{
			EnemyAI->StopAI();		//AI컨트롤러의 BehaviorTree정지

			SetActorEnableCollision(false);				//캐릭터 콜리전 false
			GetMesh()->SetHiddenInGame(false);			//캐릭터 매시 드러냄
			EnemyHPWidget->SetHiddenInGame(true);		//HPMPBar 숨김
			EnemyAnimInstance->IsDead = true;			//캐릭터 죽는 애니메이션
			SetCanBeDamaged(false);						//대미지를 입지 않음
			//플레이어 컨트롤러에 있는 적을 처치하고 얻은 경험치를 얻는 함수를 호출한다
			if (PlayerController) PlayerController->KillEnemy(EnemyStat->GetEXP());

			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
				Destroy();							//이 액터를 파괴한다
				}), DeadTimer, false);
		}
		break;

		default:
			break;
	}
}

//HPWidget 숨김 설정 ( 외부에서 사용용도 )
void APA_Enemy_Character::SetHPWidgetHidden(bool bHPWidgetHidden)
{
	if (EnemyHPWidget != nullptr)
	{
		if (bHPWidgetHidden) EnemyHPWidget->SetHiddenInGame(true);		//HPMPBar 숨김
		else EnemyHPWidget->SetHiddenInGame(false);				//HPMPBar 드러냄
	}
}