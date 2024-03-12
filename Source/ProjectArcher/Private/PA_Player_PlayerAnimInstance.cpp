// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_PlayerAnimInstance.h"
#include "PA_Player_PlayerCharacter.h"

#include "Kismet/KismetMathLibrary.h"					//Delta(Rotator)
#include "GameFramework/CharacterMovementComponent.h"	//캐릭터무브먼트



UPA_Player_PlayerAnimInstance::UPA_Player_PlayerAnimInstance()
{
	//공격 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_MONTAGE
	(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage"));
	if (FIRE_MONTAGE.Succeeded()) FireMontage = FIRE_MONTAGE.Object;
	//주문 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>SPELL_MONTAGE
	(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Cast_Montage.Cast_Montage"));
	if (SPELL_MONTAGE.Succeeded()) SpellMontage = SPELL_MONTAGE.Object;

	//프로퍼티 초기화
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

	//Pawn에 폰의 권한을 줌
	Pawn = Cast<APA_Player_PlayerCharacter>(TryGetPawnOwner());
	if (!::IsValid(Pawn)) return;

	//캐릭터가 죽지 않았을 때는 캐릭터의 스피드 변수, 점프여부 변수에 값을 기입한다.
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();				//스피드
		Velocity = Pawn->GetVelocity();								//속도
		if (Pawn)
		{
			IsInAir = Pawn->GetMovementComponent()->IsFalling();	//점프

			BaseAimRotation = Pawn->GetBaseAimRotation();			//시선

			//캐릭터 회전시키는 함수
			TurnInPlace(DeltaSeconds);
		}
	}
}

//몽타주 재생
void UPA_Player_PlayerAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage, 1.0f);
}
void UPA_Player_PlayerAnimInstance::PlaySpellMontage()
{
	Montage_Play(SpellMontage, 1.0f);
}

//노티파이
void UPA_Player_PlayerAnimInstance::AnimNotify_ResetCombo()
{
	OnResetCombo.Broadcast();
}

//캐릭터를 회전 시키는 함수
void UPA_Player_PlayerAnimInstance::TurnInPlace(float DeltaSeconds)
{
	//YawOffset설정 ( 캐릭터가 가고 있는 방향과 카메라가 보고 있는 방향의 Yaw의 각도를 구해 YawOffset에 기입한다 )
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(FRotationMatrix::MakeFromX(Pawn->GetVelocity()).Rotator(),
		BaseAimRotation).Yaw;

	//루트 본 회전( Rotate Root Bone )
	if (CurrentPawnSpeed > 0.0f || IsInAir)
	{
		//움직이거나 공중에 있을 때는 RootYawOffset을 0으로 지정하고 이동회전 슬래시 재설정
		RootYawOffset = FMath::FInterpTo(RootYawOffset, 0.0f, DeltaSeconds, 20.0f);
		MovingRotation = Pawn->GetActorRotation();
		LastMovingRotation = MovingRotation;
	}
	else
	{
		//멈춰있거나 공중에 있지 않을 때는 RootYawOffset을 계산
		LastMovingRotation = MovingRotation;
		MovingRotation = Pawn->GetActorRotation();
		RootYawOffset = RootYawOffset - UKismetMathLibrary::NormalizedDeltaRotator(MovingRotation, LastMovingRotation).Yaw;

		//캐릭터가 회전할 때
		if (GetCurveValue(TEXT("Turning")) > 0.0f)
		{
			//DistanceCurve(움직이는 양의 커브)
			LastDistanceCurve = DistanceCurve;
			DistanceCurve = GetCurveValue(TEXT("DistanceCurve"));
			DeltaDistanceCurve = DistanceCurve - LastDistanceCurve;	//DeltaSeconds마다 움직이는 각도의 양

			if (RootYawOffset > 0.0f)	//왼쪽으로 돌 때
			{
				RootYawOffset = RootYawOffset - DeltaDistanceCurve;		//RootYawOffset을 서서히 0으로 만든다
			}
			else						//오른쪽으로 돌 때
			{
				RootYawOffset = RootYawOffset + DeltaDistanceCurve;		//RootYawOffset을 서서히 0으로 만든다
			}

			//YawExcess(90이 넘어가는 초과량) 계산
			AbsRootYawOffset = UKismetMathLibrary::Abs(RootYawOffset);
			if (AbsRootYawOffset > 90.0f)
			{
				YawExcess = AbsRootYawOffset - 90.0f;
			}

			//초과된 Yaw값을 되돌려준다
			if (RootYawOffset > 0.0f)
			{
				RootYawOffset = RootYawOffset - YawExcess;		//RootYawOffset을 90으로 만든다 ( 왼쪽, 90으로 되어 왼쪽으로 돈 다음 위에 있는 코드들로 서서히 0으로 설정된다 )
			}
			else
			{
				RootYawOffset = RootYawOffset + YawExcess;		//RootYawOffset을 -90으로 만든다 ( 오른쪽, -90으로 되어 왼쪽으로 돈 다음 위에 있는 코드들로 서서히 0으로 설정된다 )
			}
		}

	}
}