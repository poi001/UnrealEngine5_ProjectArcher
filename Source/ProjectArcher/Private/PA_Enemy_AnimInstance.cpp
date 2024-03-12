// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_Character.h"

#include "GameFramework/CharacterMovementComponent.h"	//캐릭터무브먼트


UPA_Enemy_AnimInstance::UPA_Enemy_AnimInstance()
{
	//프로퍼티 초기화;
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	IsAttack = false;
	IsInAir = false;
}

void UPA_Enemy_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Pawn에 폰의 권한을 줌
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	//캐릭터가 공격중이거나 죽지 않았을 때는 캐릭터의 스피드 변수, 점프여부 변수에 값을 기입하고 아닐 때는 스피드를 0으로 설정한다.
	if (!IsDead || !IsAttack)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		IsInAir = Pawn->GetMovementComponent()->IsFalling();	//점프
	}
	else
	{
		CurrentPawnSpeed = 0.0f;
	}
}

void UPA_Enemy_AnimInstance::PlayAttackMontage()
{

}

void UPA_Enemy_AnimInstance::AnimNotify_AttackCheck()
{
	OnAttackCheckDelegate.Broadcast();
}
void UPA_Enemy_AnimInstance::AnimNotify_AttackEnd()
{
	OnAttackEndDelegate.Broadcast();
	IsAttack = false;
}