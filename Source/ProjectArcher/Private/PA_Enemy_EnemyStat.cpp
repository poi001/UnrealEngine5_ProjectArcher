// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_EnemyStat.h"
#include "PA_Setting.h"

// Sets default values for this component's properties
UPA_Enemy_EnemyStat::UPA_Enemy_EnemyStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	//프로퍼티
	Code = 0;
}


// Called when the game starts
void UPA_Enemy_EnemyStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	//초기화
	SetNewCode(Code);
}


// Called every frame
void UPA_Enemy_EnemyStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Set함수
void UPA_Enemy_EnemyStat::SetNewCode(int32 NewCode)
{
	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();

	CurrentStatData = DefaultSetting->GetEnemyStatData(NewCode + 1);
	if (CurrentStatData != nullptr)
	{
		Code = NewCode;
		MaxHP = CurrentStatData->HP;
		SetHP(CurrentStatData->HP);
		Speed = CurrentStatData->Speed;
		AttackDamage = CurrentStatData->AttackDamage;
		AttackRange = CurrentStatData->AttackRange;
		EXP = CurrentStatData->Exp;
		Gold = CurrentStatData->Gold;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyActorComponent's Stat is nullptr"));
	}
}
void UPA_Enemy_EnemyStat::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP));
}
void UPA_Enemy_EnemyStat::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChangedDelegate.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER) //KINDA_SMALL_NUMBER은 미세한 오차범위를 조정해줄 매크로 ( 0이랑 비교할 때 )
	{
		CurrentHP = 0.0f;
		OnHPIsZeroDelegate.Broadcast();
	}
}

//Get 함수
float UPA_Enemy_EnemyStat::GetHPRatio() const
{
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
}