// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_TurnToTarget.h"
#include "PA_Enemy_Controller.h"
#include "PA_Player_PlayerCharacter.h"
#include "PA_Enemy_Character.h"
#include "PA_Enemy_AnimInstance.h"

#include "BehaviorTree/BlackboardComponent.h"			//블랙보드 컴포넌트

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<APA_Enemy_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Enemy) return EBTNodeResult::Failed;

	auto Anim = Cast<UPA_Enemy_AnimInstance>(Enemy->EnemyAnimInstance);
	if (Anim->IsDead) return EBTNodeResult::Failed;

	auto Target = Cast<APA_Player_PlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APA_Enemy_Controller::TargetKey));
	if (nullptr == Target) return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Enemy->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Enemy->SetActorRotation(TargetRot);

	return EBTNodeResult::Succeeded;
}