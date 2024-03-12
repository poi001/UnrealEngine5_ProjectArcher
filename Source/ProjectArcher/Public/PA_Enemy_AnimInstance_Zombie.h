// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_AnimInstance_Zombie.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Enemy_AnimInstance_Zombie : public UPA_Enemy_AnimInstance
{
	GENERATED_BODY()

public:
	UPA_Enemy_AnimInstance_Zombie();

	//根鸥林 犁积
	virtual void PlayAttackMontage() override;
	
private:
	//根鸥林
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage;				//傍拜 根鸥林
};
