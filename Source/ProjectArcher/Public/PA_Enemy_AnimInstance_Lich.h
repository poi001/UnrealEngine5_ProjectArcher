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

	//몽타주 재생
	virtual void PlayAttackMontage() override;

	//캐릭터 등장씬 관련 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Emerge")
	bool IsEmerging;

	//Ray스킬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ray")
	bool RayOn;

	//델리게이트
	FOnRayStart OnRayStartDelegate;		//Ray 시작 델리게이트
	FOnRayEnd OnRayEndDelegate;			//Ray 끝 델리게이트
	FOnTeleport OnTeleport;				//Teleport 델리게이트

private:
	//몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage1;				//공격1 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage2;				//공격2 몽타주

	//노티파이
	UFUNCTION()
	void AnimNotify_RayStart();					//공격 판정
	UFUNCTION()
	void AnimNotify_RayEnd();					//공격 끝
	UFUNCTION()
	void AnimNotify_Teleport();					//텔레포트

};
