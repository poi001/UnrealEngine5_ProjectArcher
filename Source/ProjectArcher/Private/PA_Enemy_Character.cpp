// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Character.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_EnemyStat.h"
#include "PA_Enemy_HPBar.h"
#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_DamageWidget.h"
#include "PA_Object_PlayerArrow.h"
#include "PA_Player_PlayerController.h"

#include "Components/WidgetComponent.h"				//���� ������Ʈ
#include "Engine/DamageEvents.h"					//����� �̺�Ʈ

// Sets default values
APA_Enemy_Character::APA_Enemy_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������Ʈ ����
	EnemyStat = CreateDefaultSubobject<UPA_Enemy_EnemyStat>(TEXT("ENEMYSTAT"));
	EnemyHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ENEMYHPWIDGET"));
	DamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DAMAGEWIDGET"));

	//������Ʈ ����
	EnemyHPWidget->SetupAttachment(GetMesh());	//HP����
	DamageWidget->SetupAttachment(GetMesh());	//����� ����

	//����
		//HP����
	EnemyHPWidget->SetWidgetSpace(EWidgetSpace::Screen);				//HP ������ �׻� ��ũ���� ���̵��� ����
	static ConstructorHelpers::FClassFinder<UPA_Enemy_HPBar>UI_HUD(TEXT("/Game/Widget/UI/WBP_EnemyHPBar.WBP_EnemyHPBar_C"));
	if (UI_HUD.Succeeded())
	{
		EnemyHPWidget->SetWidgetClass(UI_HUD.Class);								//�⺻ Ŭ������ ����
		EnemyHPWidget->SetDrawSize(FVector2D(150.0f, 50.0f));						//������ ũ�� ����
	}
	EnemyHPWidget->SetHiddenInGame(true);	//HPBar����
		//����� ����
	DamageWidget->SetWidgetSpace(EWidgetSpace::Screen);					//����� ������ �׻� ��ũ���� ���̵��� ����
	static ConstructorHelpers::FClassFinder<UPA_Enemy_DamageWidget>DAMAGE_WIDGET(TEXT("/Game/Widget/Damage/WBP_DamageWidget.WBP_DamageWidget_C"));
	if (DAMAGE_WIDGET.Succeeded())
	{
		DamageWidget->SetWidgetClass(DAMAGE_WIDGET.Class);							//�⺻ Ŭ������ ����
		DamageWidget->SetDrawSize(FVector2D(256.0f, 75.0f));						//������ ũ�� ����
	}

	//AI����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	//��ȯ�ǰų� �̹� �����ϴ� ��쿡 AI��Ʈ�ѷ� ����

	//AI��Ʈ�ѷ�
	AIControllerClass = APA_Enemy_Controller::StaticClass();				//AI��Ʈ�ѷ� Ŭ���� ����

	//�ִϸ��̼� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//ĳ���� ���� ����
	CharacterState = ECharacterState::PREINIT;

	//������Ƽ ����
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
	//��Ÿ ȭ������ Ȯ��
	if (ArrowObject)
	{
		//ġ��Ÿ Ȯ��
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

	//óġ�� �÷��̾��� ��Ʈ�ѷ�
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
			SetActorHiddenInGame(true);				//���� ����
			EnemyHPWidget->SetHiddenInGame(true);	//HPBar����
			SetCanBeDamaged(false);					//����� ���� ����
		}
		break;

		case ECharacterState::READY:
		{
			EnemyAI->RunAI();		//AI��Ʈ�ѷ��� BehaviorTree�۵�

			SetActorHiddenInGame(false);			//���� �巯��
			//EnemyHPWidget->SetHiddenInGame(false);	//HPBar �巯��
			SetCanBeDamaged(true);					//����� ����

			//HP�� ���εǾ��� ��
			EnemyStat->OnHPIsZeroDelegate.AddLambda([this]()->void {SetEnemyState(ECharacterState::DEAD); });	//DEAD�� ���� ��ȯ

			auto EnemyWidget = Cast<UPA_Enemy_HPBar>(EnemyHPWidget->GetUserWidgetObject());
			if (EnemyWidget != nullptr) EnemyWidget->BindEnemyStat(EnemyStat);	//�� ���� ��������Ʈ�� ���ε���
		}
		break;

		case ECharacterState::DEAD:
		{
			EnemyAI->StopAI();		//AI��Ʈ�ѷ��� BehaviorTree����

			SetActorEnableCollision(false);				//ĳ���� �ݸ��� false
			GetMesh()->SetHiddenInGame(false);			//ĳ���� �Ž� �巯��
			EnemyHPWidget->SetHiddenInGame(true);		//HPMPBar ����
			EnemyAnimInstance->IsDead = true;			//ĳ���� �״� �ִϸ��̼�
			SetCanBeDamaged(false);						//������� ���� ����
			//�÷��̾� ��Ʈ�ѷ��� �ִ� ���� óġ�ϰ� ���� ����ġ�� ��� �Լ��� ȣ���Ѵ�
			if (PlayerController) PlayerController->KillEnemy(EnemyStat->GetEXP());

			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
				Destroy();							//�� ���͸� �ı��Ѵ�
				}), DeadTimer, false);
		}
		break;

		default:
			break;
	}
}

//HPWidget ���� ���� ( �ܺο��� ���뵵 )
void APA_Enemy_Character::SetHPWidgetHidden(bool bHPWidgetHidden)
{
	if (EnemyHPWidget != nullptr)
	{
		if (bHPWidgetHidden) EnemyHPWidget->SetHiddenInGame(true);		//HPMPBar ����
		else EnemyHPWidget->SetHiddenInGame(false);				//HPMPBar �巯��
	}
}