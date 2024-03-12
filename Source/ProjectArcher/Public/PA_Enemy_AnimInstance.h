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

	//스피드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	float CurrentPawnSpeed;

	//생사
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool IsDead;

	//공격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool IsAttack;

	//점프
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool IsInAir;

	//델리게이트
	FOnAttackCheck OnAttackCheckDelegate;	//공격 판정
	FOnAttackEnd OnAttackEndDelegate;		//공격 끝

	//몽타주 재생
	virtual void PlayAttackMontage();			//발사 몽타주 재생

private:
	//노티파이
	UFUNCTION()
	void AnimNotify_AttackCheck();					//공격 판정
	UFUNCTION()
	void AnimNotify_AttackEnd();					//공격 끝
};
