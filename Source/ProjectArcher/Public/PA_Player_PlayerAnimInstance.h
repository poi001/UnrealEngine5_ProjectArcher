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
	//몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* FireMontage;			//발사 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* SpellMontage;			//주문 몽타주

	//회전
	FRotator MovingRotation = FRotator::ZeroRotator;	//캐릭터가 움직이는 도중의 회전율
	FRotator LastMovingRotation = FRotator::ZeroRotator;//마지막 회전율
	float DistanceCurve = 0.0f;							//캐릭터가 회전하는 양
	float LastDistanceCurve = 0.0f;						//마지막 회전양
	float DeltaDistanceCurve = 0.0f;					//델타 커브
	float AbsRootYawOffset = 0.0f;						//RootYawOffset의 절대값
	float YawExcess = 0.0f;								//AbsRootYawOffset - 90 하여 남는 값

	//노티파이
	UFUNCTION()
	void AnimNotify_ResetCombo();					//발사 노티파이

	//캐릭터를 회전 시키는 함수
	void TurnInPlace(float DeltaSeconds);

public:
	//스피드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	//속도 ( Velocity )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	FVector Velocity;

	//점프
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	//생사
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsDead;

	//조준 ( 조준하면서 움직이는 애니메이션의 완성도를 위한 변수 )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float YawOffset;			//캐릭터의 회전율에서 카메라의 회전율까지의 Yaw의 각도 ( 조준하면서 움직일 때 사용 )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	FRotator BaseAimRotation;	//카메라가 보고있는 회전율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float RootYawOffset;		//본의 Yaw회전율 변수

	//폰
	UPROPERTY()
	class APA_Player_PlayerCharacter* Pawn;

	//델리게이트
	FOnResetCombo OnResetCombo;		//공격 끝났을 때, 델리게이트

	//몽타주 재생
	void PlayFireMontage();			//발사 몽타주 재생
	void PlaySpellMontage();		//주문 몽타주 재생
};
