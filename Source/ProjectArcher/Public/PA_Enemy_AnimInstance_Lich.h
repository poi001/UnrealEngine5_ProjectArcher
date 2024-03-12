// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_AnimInstance_Lich.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRayStart)
DECLARE_MULTICAST_DELEGATE(FOnRayEnd)
DECLARE_MULTICAST_DELEGATE(FOnTeleport)

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Enemy_AnimInstance_Lich : public UPA_Enemy_AnimInstance
{
	GENERATED_BODY()
	
public:
	UPA_Enemy_AnimInstance_Lich();

	//��Ÿ�� ���
	virtual void PlayAttackMontage() override;

	//ĳ���� ����� ���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Emerge")
	bool IsEmerging;

	//Ray��ų
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ray")
	bool RayOn;

	//��������Ʈ
	FOnRayStart OnRayStartDelegate;		//Ray ���� ��������Ʈ
	FOnRayEnd OnRayEndDelegate;			//Ray �� ��������Ʈ
	FOnTeleport OnTeleport;				//Teleport ��������Ʈ

private:
	//��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage1;				//����1 ��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage2;				//����2 ��Ÿ��

	//��Ƽ����
	UFUNCTION()
	void AnimNotify_RayStart();					//���� ����
	UFUNCTION()
	void AnimNotify_RayEnd();					//���� ��
	UFUNCTION()
	void AnimNotify_Teleport();					//�ڷ���Ʈ

};
