// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Character_Zombie.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_EnemyStat.h"

#include "Components/WidgetComponent.h"					//���� ������Ʈ
#include "DrawDebugHelpers.h"							//��ο�����
#include "Engine/DamageEvents.h"						//DamageEvent
#include "GameFramework/CharacterMovementComponent.h"	//ĳ���͹����Ʈ


APA_Enemy_Character_Zombie::APA_Enemy_Character_Zombie()
{
	//�⺻ �ִ� �ν��Ͻ� ����
	static ConstructorHelpers::FClassFinder<UPA_Enemy_AnimInstance> ENEMY_ANIM(TEXT("/Game/Character/Enemy/AnimBP/AnimBP_Zombie.AnimBP_Zombie_C"));
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	//����
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));			//��ġ�� ĳ���� �Ӹ�����
	DamageWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));				//��ġ�� HPBar ����
}

void APA_Enemy_Character_Zombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnimInstance = Cast<UPA_Enemy_AnimInstance>(GetMesh()->GetAnimInstance());
	//���� ���� ��������Ʈ ����
	EnemyAnimInstance->OnAttackCheckDelegate.AddUObject(this, &APA_Enemy_Character_Zombie::AttackCheck);
	//���� ���� �� ��������Ʈ ����
	EnemyAnimInstance->OnAttackEndDelegate.AddUObject(this, &APA_Enemy_Character_Zombie::AttackEnd);
}

void APA_Enemy_Character_Zombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AI ������ ���� AI�� �ִ´�
	EnemyAI = Cast<APA_Enemy_Controller>(NewController);
}
//start
void APA_Enemy_Character_Zombie::BeginPlay()
{
	Super::BeginPlay();

	//�� ĳ���� ���� ���� //�񵿱� �������� �ҷ����� �Ϸ�� ������ LOADING ���·� �ִٰ� READY�� �ٲ۴�
	EnemyStat->SetNewCode(1);
	GetCharacterMovement()->MaxWalkSpeed = EnemyStat->GetSpeed();		//�̵��ӵ� ����
	EnemyAI->SetZombie();
	SetEnemyState(ECharacterState::READY);
}

void APA_Enemy_Character_Zombie::AttackCheck()
{
	FHitResult HitResult;
	//FCollisionQueryParams ����
	//1. �±� ���̹�, 2. ���� ���⼺ ����(�������� ������ �ƴ϶�� false ��õ), 3. ���� �ִ� ������ ����
	FCollisionQueryParams Params(NAME_None, false, this);
	float AttackRange = EnemyStat->GetAttackRange();

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,														//������ �浹�� Ž���� ��� ���õ� ������ ���� ����ü
		GetActorLocation(),												//Ž���� ������ ��ġ
		GetActorLocation() + GetActorForwardVector() * AttackRange,		//Ž���� ���� ��ġ
		FQuat::Identity,												//Ž���� ����� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel4,						//���� �浹 ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(AttackRadius),						//Ž���� ����� ���� ����
		Params															//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü
	);
	/*
#if ENABLE_DRAW_DEBUG	//����� ������� ������ ����
	FVector TraceVec = GetActorForwardVector() * AttackRange;			//Ʈ���̽� ����
	FVector Center = GetActorLocation() + TraceVec * 0.5f;				//ĸ���� ��� ��
	float HalfHeight = AttackRange * 0.5f + AttackRadius;				//������ ����
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();	//ĸ���� ȸ��
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;			//ĸ���� ����
	float DebugLifeTime = 3.0f;											//ĸ���� ��Ÿ�� ���� �ð�

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif
*/
	if (bResult)
	{
		if (HitResult.GetActor())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(EnemyStat->GetAttackDamage(), DamageEvent, GetController(), this);//��뿡�� ���ظ� �ش�
		}
	}
}

void APA_Enemy_Character_Zombie::AttackEnd()
{
	EnemyAnimInstance->IsAttack = false;
}