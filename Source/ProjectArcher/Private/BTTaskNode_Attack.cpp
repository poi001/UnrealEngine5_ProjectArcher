// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "PA_Enemy_Character.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_AnimInstance.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	bNotifyTick = true;		//TickTask�� ���� ���� true
	IsAttacking = false;
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//Enemy�� nullptr�̸� ���и� ��ȯ�Ѵ�
	auto Enemy = Cast<APA_Enemy_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Enemy) return EBTNodeResult::Failed;

	//���� �����ൿ�� �����Ų��.
	auto EnemyAnimInstance = Cast<UPA_Enemy_AnimInstance>(Enemy->EnemyAnimInstance);
	EnemyAnimInstance->PlayAttackMontage();
	IsAttacking = true;
	EnemyAnimInstance->OnAttackEndDelegate.AddLambda([this]()->void { IsAttacking = false; });

	return EBTNodeResult::InProgress;	//�½�ũ�� �������� ����
}

void UBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	//FinishLatentTask�� �������� ���¸� ������ ���·� �ٲٴ� �Լ��̴�
	}
}