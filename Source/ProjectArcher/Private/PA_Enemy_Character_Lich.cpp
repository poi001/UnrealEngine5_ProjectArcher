// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Character_Lich.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_EnemyStat.h"
#include "PA_Enemy_AnimInstance_Lich.h"

#include "Components/WidgetComponent.h"					//���� ������Ʈ
#include "DrawDebugHelpers.h"							//��ο�����
#include "Engine/DamageEvents.h"						//DamageEvent
#include "GameFramework/CharacterMovementComponent.h"	//ĳ���͹����Ʈ
#include "Kismet/GameplayStatics.h"						//Kimset


APA_Enemy_Character_Lich::APA_Enemy_Character_Lich()
{
	//�⺻ �ִ� �ν��Ͻ� ����
	static ConstructorHelpers::FClassFinder<UPA_Enemy_AnimInstance> ENEMY_ANIM(TEXT("/Game/Character/Enemy/AnimBP/AnimBP_Lich.AnimBP_Lich_C"));
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	//����
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));			//��ġ�� ĳ���� �Ӹ�����
	DamageWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 260.0f));				//��ġ�� HPBar ����

	//������Ƽ
	LichDamage = 0.0f;
}

void APA_Enemy_Character_Lich::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnimInstance = Cast<UPA_Enemy_AnimInstance>(GetMesh()->GetAnimInstance());
	//���� ���� ��������Ʈ ����
	EnemyAnimInstance->OnAttackCheckDelegate.AddUObject(this, &APA_Enemy_Character_Lich::AttackCheck);
	//���� ���� �� ��������Ʈ ����
	EnemyAnimInstance->OnAttackEndDelegate.AddUObject(this, &APA_Enemy_Character_Lich::AttackEnd);
	//Ray��ų ��������Ʈ

}

void APA_Enemy_Character_Lich::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AI ������ ���� AI�� �ִ´�
	EnemyAI = Cast<APA_Enemy_Controller>(NewController);
}
//start
void APA_Enemy_Character_Lich::BeginPlay()
{
	Super::BeginPlay();

	//�� ĳ���� ���� ���� //�񵿱� �������� �ҷ����� �Ϸ�� ������ LOADING ���·� �ִٰ� READY�� �ٲ۴�
	EnemyStat->SetNewCode(2);
	GetCharacterMovement()->MaxWalkSpeed = EnemyStat->GetSpeed();		//�̵��ӵ� ����
	LichDamage = EnemyStat->GetAttackDamage();							//���ݷ� ����
	EnemyAI->SetLich();

	//Lich ĳ������ Ray ��ų ��������Ʈ
	auto LichAnimInstace = Cast<UPA_Enemy_AnimInstance_Lich>(EnemyAnimInstance);
	LichAnimInstace->OnRayStartDelegate.AddUObject(this, &APA_Enemy_Character_Lich::OnRaySkill);
	LichAnimInstace->OnRayEndDelegate.AddUObject(this, &APA_Enemy_Character_Lich::OffRaySkill);

	//Lich ĳ������ �ڷ���Ʈ ��������Ʈ
	LichAnimInstace->OnTeleport.AddUObject(this, &APA_Enemy_Character_Lich::LichTeleportToPlayer);
	
	//�ִ� �ν��Ͻ� ��üũ
	if (EnemyAnimInstance == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("EnemyAnimInstance is nullptr"));
	}

	//Ÿ�̸� �ڵ�
	FTimerHandle EmergenceTimerHandle;
	//Lich�� ������ ��
	GetWorld()->GetTimerManager().SetTimer(
		EmergenceTimerHandle,
		FTimerDelegate::CreateLambda(
			[this, LichAnimInstace]()->void
			{
				LichAnimInstace->IsEmerging = false;
				SetEnemyState(ECharacterState::READY);
			}),
		2.25f, false);
}

void APA_Enemy_Character_Lich::AttackCheck()
{
	FHitResult HitResult;

	//FCollisionQueryParams ����
	//1. �±� ���̹�, 2. ���� ���⼺ ����(�������� ������ �ƴ϶�� false ��õ), 3. ���� �ִ� ������ ����
	FCollisionQueryParams Params(NAME_None, false, this);

	//���� ��Ÿ�
	float AttackRange = EnemyStat->GetAttackRange();

	//Ʈ���̽� ����
	FVector TraceVec = GetActorForwardVector() * AttackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,														//������ �浹�� Ž���� ��� ���õ� ������ ���� ����ü
		GetActorLocation(),												//Ž���� ������ ��ġ
		GetActorLocation() + TraceVec,									//Ž���� ���� ��ġ
		FRotationMatrix::MakeFromY(TraceVec).ToQuat(),					//Ž���� ����� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel4,						//���� �浹 ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(AttackRadius),						//Ž���� ����� ���� ����
		Params															//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü
	);
	/*
#if ENABLE_DRAW_DEBUG	//����� ������� ������ ����
	FVector Center = GetActorLocation() + TraceVec * 0.5f;				//ĸ���� ��� ��
	float HalfHeight = AttackRange * 0.5f + AttackRadius;				//������ ����
	FQuat CapsuleRot = FRotationMatrix::MakeFromY(TraceVec).ToQuat();	//ĸ���� ȸ��
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

void APA_Enemy_Character_Lich::AttackEnd()
{
	EnemyAnimInstance->IsAttack = false;
}

void APA_Enemy_Character_Lich::LichTeleportToPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Teleport"));
	SetActorLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
}