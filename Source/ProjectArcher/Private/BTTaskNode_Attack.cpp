// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "PA_Enemy_Character.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_AnimInstance.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	bNotifyTick = true;		//TickTask를 쓰기 위해 true
	IsAttacking = false;
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//Enemy가 nullptr이면 실패를 반환한다
	auto Enemy = Cast<APA_Enemy_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Enemy) return EBTNodeResult::Failed;

	//적의 공격행동을 실행시킨다.
	auto EnemyAnimInstance = Cast<UPA_Enemy_AnimInstance>(Enemy->EnemyAnimInstance);
	EnemyAnimInstance->PlayAttackMontage();
	IsAttacking = true;
	EnemyAnimInstance->OnAttackEndDelegate.AddLambda([this]()->void { IsAttacking = false; });

	return EBTNodeResult::InProgress;	//태스크가 진행중인 상태
}

void UBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	//FinishLatentTask는 진행중인 상태를 설정한 상태로 바꾸는 함수이다
	}
}