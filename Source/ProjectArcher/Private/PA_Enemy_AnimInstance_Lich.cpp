// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_AnimInstance_Lich.h"
#include "PA_Enemy_Controller.h"

UPA_Enemy_AnimInstance_Lich::UPA_Enemy_AnimInstance_Lich()
{
	//���� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE1(TEXT("/Game/UndeadPack/Lich/Animations/Montage_Attack1.Montage_Attack1"));
	if (ATTACK_MONTAGE1.Succeeded()) AttackMontage1 = ATTACK_MONTAGE1.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE2(TEXT("/Game/UndeadPack/Lich/Animations/Montage_Attack2.Montage_Attack2"));
	if (ATTACK_MONTAGE2.Succeeded()) AttackMontage2 = ATTACK_MONTAGE2.Object;

	//������Ƽ
	IsEmerging = true;
	RayOn = false;
}

//��Ÿ�� ���
void UPA_Enemy_AnimInstance_Lich::PlayAttackMontage()
{
	auto EnemyAI = Cast<APA_Enemy_Controller>(TryGetPawnOwner()->GetController());
	int32 RandomAttack = EnemyAI->GetAttackNumber();
	RandomAttack = FMath::Clamp(RandomAttack, 0, 2);

	switch (RandomAttack)
	{
	case 0: {
		Montage_Play(AttackMontage1, 1.0f);
		IsAttack = true;
		break;
	}

	case 1: {
		Montage_Play(AttackMontage2, 1.0f);
		IsAttack = true;
		break;
	}

	case 2: {
		IsAttack = true;
		RayOn = true;

		//Ÿ�̸� �ڵ�
		FTimerHandle EmergenceTimerHandle;

		//Delay
		GetWorld()->GetTimerManager().SetTimer(
			EmergenceTimerHandle,
			FTimerDelegate::CreateLambda(
				[this]()->void
				{
					RayOn = false;
				}),
			2.6f + 3.0f, false);	//2.6�� �غ��ϴ� �ð�

		break;
	}

	default:
		break;
	}
}

//��������Ʈ
void UPA_Enemy_AnimInstance_Lich::AnimNotify_RayStart()
{
	OnRayStartDelegate.Broadcast();
}
void UPA_Enemy_AnimInstance_Lich::AnimNotify_RayEnd()
{
	OnRayEndDelegate.Broadcast();
}
void UPA_Enemy_AnimInstance_Lich::AnimNotify_Teleport()
{
	OnTeleport.Broadcast();
}