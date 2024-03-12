// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_Character.h"

#include "GameFramework/CharacterMovementComponent.h"	//ĳ���͹����Ʈ


UPA_Enemy_AnimInstance::UPA_Enemy_AnimInstance()
{
	//������Ƽ �ʱ�ȭ;
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	IsAttack = false;
	IsInAir = false;
}

void UPA_Enemy_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Pawn�� ���� ������ ��
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	//ĳ���Ͱ� �������̰ų� ���� �ʾ��� ���� ĳ������ ���ǵ� ����, �������� ������ ���� �����ϰ� �ƴ� ���� ���ǵ带 0���� �����Ѵ�.
	if (!IsDead || !IsAttack)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		IsInAir = Pawn->GetMovementComponent()->IsFalling();	//����
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