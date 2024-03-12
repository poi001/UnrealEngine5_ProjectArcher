// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "PA_DamageType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_DamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	virtual void ProcessDamage();
};
