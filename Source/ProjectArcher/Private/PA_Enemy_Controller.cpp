// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Controller.h"
#include "PA_Enemy_Character.h"

#include "BehaviorTree/BehaviorTree.h"					//비헤이어 트리	( Build.cs에서 AIModule추가 해야 함 )
#include "BehaviorTree/BlackboardData.h"				//블랙보드
#include "BehaviorTree/BlackboardComponent.h"			//블랙보드 컴포넌트

const FName APA_Enemy_Controller::TargetKey(TEXT("Target"));				//TargetKey의 설정
const FName APA_Enemy_Controller::IsDetectedKey(TEXT("IsDetected"));		//IsDetectedKey의 설정
const FName APA_Enemy_Controller::AttackNumber(TEXT("AttackNumber"));		//AttackNumber의 설정

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
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);	//BrainComponent는 Behavior를 담당하는 컴포넌트
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);	//EBTStopMode::Force = 무조건 Stop, Safe는 안전하게
	}
}

//Set
void APA_Enemy_Controller::SetIsDetectedKey(bool Set)
{
	Blackboard->SetValueAsBool(IsDetectedKey, Set);
}
void APA_Enemy_Controller::SetGoblin()
{
	//블랙보드
	BBAsset = Cast<UBlackboardData>(StaticLoadObject(UBlackboardData::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Goblin/BB_Goblin.BB_Goblin")));
	//비헤이어 트리
	BTAsset = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Goblin/BT_Goblin.BT_Goblin")));
}
void APA_Enemy_Controller::SetZombie()
{
	//블랙보드
	BBAsset = Cast<UBlackboardData>(StaticLoadObject(UBlackboardData::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Zombie/BB_Zombie.BB_Zombie")));
	//비헤이어 트리
	BTAsset = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Zombie/BT_Zombie.BT_Zombie")));
}
void APA_Enemy_Controller::SetLich()
{
	//블랙보드
	BBAsset = Cast<UBlackboardData>(StaticLoadObject(UBlackboardData::StaticClass(), nullptr, TEXT("/Game/Character/Enemy/AI/Lich/BB_Lich.BB_Lich")));
	//비헤이어 트리
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