// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PA_Enemy_Character.h"
#include "PA_Enemy_Character_Lich.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API APA_Enemy_Character_Lich : public APA_Enemy_Character
{
	GENERATED_BODY()

public:
	APA_Enemy_Character_Lich();

	//초기화 이후
	virtual void PostInitializeComponents() override;
	//이 액터가 컨트롤러를 소유했을 때의 호출 함수
	virtual void PossessedBy(AController* NewController) override;

private:
	virtual void BeginPlay() override;

	virtual void AttackCheck() override;
	virtual void AttackEnd() override;

	//공격 판정 반지름
	float AttackRadius = 75.0f;

	//공격 대미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float LichDamage;

protected:
	//Ray스킬
	UFUNCTION(BlueprintImplementableEvent)
	void OnRaySkill();
	UFUNCTION(BlueprintImplementableEvent)
	void OffRaySkill();
	
	//텔레포트
	void LichTeleportToPlayer();
};
