// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PA_Enemy_AnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackCheck)
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd)

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Enemy_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPA_Enemy_AnimInstance();

	//Tick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//���ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	float CurrentPawnSpeed;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool IsDead;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool IsAttack;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool IsInAir;

	//��������Ʈ
	FOnAttackCheck OnAttackCheckDelegate;	//���� ����
	FOnAttackEnd OnAttackEndDelegate;		//���� ��

	//��Ÿ�� ���
	virtual void PlayAttackMontage();			//�߻� ��Ÿ�� ���

private:
	//��Ƽ����
	UFUNCTION()
	void AnimNotify_AttackCheck();					//���� ����
	UFUNCTION()
	void AnimNotify_AttackEnd();					//���� ��
};
