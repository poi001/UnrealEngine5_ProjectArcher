// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetAttackNum.h"
#include "PA_Enemy_Character.h"
#include "PA_Enemy_Controller.h"
//#include "PA_Enemy_AnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"			//������ ������Ʈ

UBTTaskNode_SetAttackNum::UBTTaskNode_SetAttackNum()
{

}

EBTNodeResult::Type UBTTaskNode_SetAttackNum::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//Enemy�� nullptr�̸� ���и� ��ȯ�Ѵ�
	auto Enemy = Cast<APA_Enemy_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Enemy) return EBTNodeResult::Failed;

	//AI��Ʈ�ѷ��� �ִ� AttackNumber�� �����Ѵ�
	auto EnemyAI = Cast<APA_Enemy_Controller>(Enemy->EnemyAI);
	if (nullptr == EnemyAI) return EBTNodeResult::Failed;
	int32 RandomAttack = FMath::RandRange(0, 2);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(APA_Enemy_Controller::AttackNumber, RandomAttack);

	return EBTNodeResult::Succeeded;
}