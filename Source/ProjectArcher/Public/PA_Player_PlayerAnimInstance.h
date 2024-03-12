// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PA_Player_PlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResetCombo)

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Player_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPA_Player_PlayerAnimInstance();

	//Tick
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//BeginPlay
	//virtual void NativeBeginPlay() override;
	//Initialize
	//virtual void NativeInitializeAnimation() override;
	//PreInitialize
	//virtual void NativeUninitializeAnimation() override;
	//PostInitialize
	//virtual void NativePostEvaluateAnimation() override;

private:
	//��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* FireMontage;			//�߻� ��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SpellMontage;			//�ֹ� ��Ÿ��

	//ȸ��
	FRotator MovingRotation = FRotator::ZeroRotator;	//ĳ���Ͱ� �����̴� ������ ȸ����
	FRotator LastMovingRotation = FRotator::ZeroRotator;//������ ȸ����
	float DistanceCurve = 0.0f;							//ĳ���Ͱ� ȸ���ϴ� ��
	float LastDistanceCurve = 0.0f;						//������ ȸ����
	float DeltaDistanceCurve = 0.0f;					//��Ÿ Ŀ��
	float AbsRootYawOffset = 0.0f;						//RootYawOffset�� ���밪
	float YawExcess = 0.0f;								//AbsRootYawOffset - 90 �Ͽ� ���� ��

	//��Ƽ����
	UFUNCTION()
	void AnimNotify_ResetCombo();					//�߻� ��Ƽ����

	//ĳ���͸� ȸ�� ��Ű�� �Լ�
	void TurnInPlace(float DeltaSeconds);

public:
	//���ǵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	//�ӵ� ( Velocity )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	FVector Velocity;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsDead;

	//���� ( �����ϸ鼭 �����̴� �ִϸ��̼��� �ϼ����� ���� ���� )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float YawOffset;			//ĳ������ ȸ�������� ī�޶��� ȸ���������� Yaw�� ���� ( �����ϸ鼭 ������ �� ��� )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	FRotator BaseAimRotation;	//ī�޶� �����ִ� ȸ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float RootYawOffset;		//���� Yawȸ���� ����

	//��
	UPROPERTY()
	class APA_Player_PlayerCharacter* Pawn;

	//��������Ʈ
	FOnResetCombo OnResetCombo;		//���� ������ ��, ��������Ʈ

	//��Ÿ�� ���
	void PlayFireMontage();			//�߻� ��Ÿ�� ���
	void PlaySpellMontage();		//�ֹ� ��Ÿ�� ���
};
