// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SetAttackNum.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UBTTaskNode_SetAttackNum : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_SetAttackNum();

	//�½�ũ�� ��� ���� �����ϴ� �����Լ�
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
