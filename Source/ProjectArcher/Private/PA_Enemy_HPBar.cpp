// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_HPBar.h"
#include "PA_Enemy_EnemyStat.h"

#include "Components/ProgressBar.h"			//프로그레스 바


void UPA_Enemy_HPBar::NativeConstruct()
{
	Super::NativeConstruct();

	//HP 프로그레스 바
	PB_HPBar = Cast<UProgressBar>(GetWidgetFromName(FName("PB_HPBar")));
	if (PB_HPBar == nullptr) UE_LOG(LogTemp, Error, TEXT("Enemy`s HPWidget is nullptr"));

	UpdateHPBar();
}

void UPA_Enemy_HPBar::UpdateHPBar()
{
	if (CurrentEnemyStat.IsValid())
	{
		if (PB_HPBar != nullptr) PB_HPBar->SetPercent(CurrentEnemyStat->GetHPRatio());
	}
}

void UPA_Enemy_HPBar::BindEnemyStat(UPA_Enemy_EnemyStat* NewEnemyStat)
{
	if (NewEnemyStat != nullptr)
	{
		CurrentEnemyStat = NewEnemyStat;
		NewEnemyStat->OnHPChangedDelegate.AddUObject(this, &UPA_Enemy_HPBar::UpdateHPBar);
		UpdateHPBar();
	}
}