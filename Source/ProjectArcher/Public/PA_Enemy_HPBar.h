// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PA_Enemy_HPBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Enemy_HPBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* PB_HPBar;

public:
	void UpdateHPBar();
	void BindEnemyStat(class UPA_Enemy_EnemyStat* NewEnemyStat);

private:
	TWeakObjectPtr<class UPA_Enemy_EnemyStat> CurrentEnemyStat;
};
