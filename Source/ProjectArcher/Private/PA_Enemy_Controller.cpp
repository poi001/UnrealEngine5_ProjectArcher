// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Controller.h"
#include "PA_Enemy_Character.h"

#include "BehaviorTree/BehaviorTree.h"					//�����̾� Ʈ��	( Build.cs���� AIModule�߰� �ؾ� �� )
#include "BehaviorTree/BlackboardData.h"				//������
#include "BehaviorTree/BlackboardComponent.h"			//������ ������Ʈ

const FName APA_Enemy_Controller::TargetKey(TEXT("Target"));				//TargetKey�� ����
const FName APA_Enemy_Controller::IsDetectedKey(TEXT("IsDetected"));		//IsDetectedKey�� ����
const FName APA_Enemy_Controller::AttackNumber(TEXT("AttackNumber"));		//AttackNumber�� ����

APA_Enemy_Controller::APA_Enemy_Controller()
{

}

void APA_Enemy_Controller::RunAI()
{

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldn`t run behavior tree!"));
		}
	}
}
void APA_Enemy_Controller::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);	//BrainComponent�� Behavior�� ����ϴ� ������Ʈ
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);	//EBTStopMode::Force = ������ Stop, Safe�� �����ϰ�
	}
}

//Set
void APA_Enemy_Controller::SetIsDetectedKey(bool Set)
{
	Blackboard->SetValueAsBool(IsDetectedKey, Set);
}
void APA_Enemy_Controller::SetGoblin()
{
	//������
	BBAsset = Cast<UBlackboardData>(StaticLoadObject(UBlackboardData::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Goblin/BB_Goblin.BB_Goblin")));
	//�����̾� Ʈ��
	BTAsset = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Goblin/BT_Goblin.BT_Goblin")));
}
void APA_Enemy_Controller::SetZombie()
{
	//������
	BBAsset = Cast<UBlackboardData>(StaticLoadObject(UBlackboardData::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Zombie/BB_Zombie.BB_Zombie")));
	//�����̾� Ʈ��
	BTAsset = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Zombie/BT_Zombie.BT_Zombie")));
}
void APA_Enemy_Controller::SetLich()
{
	//������
	BBAsset = Cast<UBlackboardData>(StaticLoadObject(UBlackboardData::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Lich/BB_Lich.BB_Lich")));
	//�����̾� Ʈ��
	BTAsset = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Lich/BT_Lich.BT_Lich")));
}

//Get
const bool APA_Enemy_Controller::GetIsDetectedKey()
{
	return Blackboard->GetValueAsBool(IsDetectedKey);
}
const int32 APA_Enemy_Controller::GetAttackNumber()
{
	return Blackboard->GetValueAsInt(AttackNumber);
}