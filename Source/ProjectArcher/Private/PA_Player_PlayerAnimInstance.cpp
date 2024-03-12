// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_PlayerAnimInstance.h"
#include "PA_Player_PlayerCharacter.h"

#include "Kismet/KismetMathLibrary.h"					//Delta(Rotator)
#include "GameFramework/CharacterMovementComponent.h"	//ĳ���͹����Ʈ



UPA_Player_PlayerAnimInstance::UPA_Player_PlayerAnimInstance()
{
	//���� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_MONTAGE
	(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage"));
	if (FIRE_MONTAGE.Succeeded()) FireMontage = FIRE_MONTAGE.Object;
	//�ֹ� ��Ÿ��
	static ConstructorHelpers::FObjectFinder<UAnimMontage>SPELL_MONTAGE
	(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Cast_Montage.Cast_Montage"));
	if (SPELL_MONTAGE.Succeeded()) SpellMontage = SPELL_MONTAGE.Object;

	//������Ƽ �ʱ�ȭ
	CurrentPawnSpeed = 0.0f;
	Velocity = FVector::ZeroVector;
	IsInAir = false;
	IsDead = false;
	YawOffset = 0.0f;
	BaseAimRotation = FRotator::ZeroRotator;
	RootYawOffset = 0.0f;
}

void UPA_Player_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Pawn�� ���� ������ ��
	Pawn = Cast<APA_Player_PlayerCharacter>(TryGetPawnOwner());
	if (!::IsValid(Pawn)) return;

	//ĳ���Ͱ� ���� �ʾ��� ���� ĳ������ ���ǵ� ����, �������� ������ ���� �����Ѵ�.
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();				//���ǵ�
		Velocity = Pawn->GetVelocity();								//�ӵ�
		if (Pawn)
		{
			IsInAir = Pawn->GetMovementComponent()->IsFalling();	//����

			BaseAimRotation = Pawn->GetBaseAimRotation();			//�ü�

			//ĳ���� ȸ����Ű�� �Լ�
			TurnInPlace(DeltaSeconds);
		}
	}
}

//��Ÿ�� ���
void UPA_Player_PlayerAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage, 1.0f);
}
void UPA_Player_PlayerAnimInstance::PlaySpellMontage()
{
	Montage_Play(SpellMontage, 1.0f);
}

//��Ƽ����
void UPA_Player_PlayerAnimInstance::AnimNotify_ResetCombo()
{
	OnResetCombo.Broadcast();
}

//ĳ���͸� ȸ�� ��Ű�� �Լ�
void UPA_Player_PlayerAnimInstance::TurnInPlace(float DeltaSeconds)
{
	//YawOffset���� ( ĳ���Ͱ� ���� �ִ� ����� ī�޶� ���� �ִ� ������ Yaw�� ������ ���� YawOffset�� �����Ѵ� )
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(FRotationMatrix::MakeFromX(Pawn->GetVelocity()).Rotator(),
		BaseAimRotation).Yaw;

	//��Ʈ �� ȸ��( Rotate Root Bone )
	if (CurrentPawnSpeed > 0.0f || IsInAir)
	{
		//�����̰ų� ���߿� ���� ���� RootYawOffset�� 0���� �����ϰ� �̵�ȸ�� ������ �缳��
		RootYawOffset = FMath::FInterpTo(RootYawOffset, 0.0f, DeltaSeconds, 20.0f);
		MovingRotation = Pawn->GetActorRotation();
		LastMovingRotation = MovingRotation;
	}
	else
	{
		//�����ְų� ���߿� ���� ���� ���� RootYawOffset�� ���
		LastMovingRotation = MovingRotation;
		MovingRotation = Pawn->GetActorRotation();
		RootYawOffset = RootYawOffset - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, LastMovingRotation).Yaw;

		//ĳ���Ͱ� ȸ���� ��
		if (GetCurveValue(TEXT("Turning")) > 0.0f)
		{
			//DistanceCurve(�����̴� ���� Ŀ��)
			LastDistanceCurve = DistanceCurve;
			DistanceCurve = GetCurveValue(TEXT("DistanceCurve"));
			DeltaDistanceCurve = DistanceCurve - LastDistanceCurve;	//DeltaSeconds���� �����̴� ������ ��

			if (RootYawOffset > 0.0f)	//�������� �� ��
			{
				RootYawOffset = RootYawOffset - DeltaDistanceCurve;		//RootYawOffset�� ������ 0���� �����
			}
			else						//���������� �� ��
			{
				RootYawOffset = RootYawOffset + DeltaDistanceCurve;		//RootYawOffset�� ������ 0���� �����
			}

			//YawExcess(90�� �Ѿ�� �ʰ���) ���
			AbsRootYawOffset = UKismetMathLibrary::Abs(RootYawOffset);
			if (AbsRootYawOffset > 90.0f)
			{
				YawExcess = AbsRootYawOffset - 90.0f;
			}

			//�ʰ��� Yaw���� �ǵ����ش�
			if (RootYawOffset > 0.0f)
			{
				RootYawOffset = RootYawOffset - YawExcess;		//RootYawOffset�� 90���� ����� ( ����, 90���� �Ǿ� �������� �� ���� ���� �ִ� �ڵ��� ������ 0���� �����ȴ� )
			}
			else
			{
				RootYawOffset = RootYawOffset + YawExcess;		//RootYawOffset�� -90���� ����� ( ������, -90���� �Ǿ� �������� �� ���� ���� �ִ� �ڵ��� ������ 0���� �����ȴ� )
			}
		}

	}
}