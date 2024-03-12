// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_PlayerStat.h"
#include "PA_Setting.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPA_Player_PlayerStat::UPA_Player_PlayerStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//설정
	Level = 1;
}

// Called when the game starts
void UPA_Player_PlayerStat::BeginPlay()
{
	Super::BeginPlay();

	SetNewLevel(Level);
}

void UPA_Player_PlayerStat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//1초마다 체력, 마나 회복
	if (TickBasket >= 1.0f)
	{
		SetDamage(HPRegen * -1.0f);
		UseMana(MPRegen * -1.0f);
		TickBasket = 0.0f;
	}
	else
	{
		TickBasket += DeltaTime;
	}
}

//Set
void UPA_Player_PlayerStat::SetNewLevel(int32 NewLevel)
{
	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();

	CurrentStatData = DefaultSetting->GetPlayerStatData(NewLevel);
	if (CurrentStatData != nullptr)
	{
		Level = NewLevel;
		MaxHP = CurrentStatData->HP;
		SetHP(CurrentStatData->HP);
		HPRegen = CurrentStatData->HPRegen;
		MaxMP = CurrentStatData->MP;
		SetMP(CurrentStatData->MP);
		MPRegen = CurrentStatData->MPRegen;
		Speed = 600.0f;
		AttackDamage = CurrentStatData->AttackDamage;
		AttackSpeed = CurrentStatData->AttackSpeed;
		MaxEXP = CurrentStatData->Exp;
		CurrentEXP = 0.0f;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerActorComponent's Stat is nullptr"));
	}
}
void UPA_Player_PlayerStat::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP));
}
void UPA_Player_PlayerStat::UseMana(float UsedMana)
{
	if (CurrentMP >= UsedMana) SetMP(FMath::Clamp<float>(CurrentMP - UsedMana, 0.0f, MaxMP));
}
void UPA_Player_PlayerStat::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChangedDelegate.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER) //KINDA_SMALL_NUMBER은 미세한 오차범위를 조정해줄 매크로 ( 0이랑 비교할 때 )
	{
		CurrentHP = 0.0f;
		OnHPIsZeroDelegate.Broadcast();
	}
}
void UPA_Player_PlayerStat::SetMP(float NewMP)
{
	CurrentMP = NewMP;
	OnMPChangedDelegate.Broadcast();
	if (CurrentMP < KINDA_SMALL_NUMBER) //KINDA_SMALL_NUMBER은 미세한 오차범위를 조정해줄 매크로 ( 0이랑 비교할 때 )
	{
		CurrentMP = 0.0f;
	}
}
void UPA_Player_PlayerStat::SetEXP(float AddEXP)
{
	CurrentEXP += AddEXP;
	if (MaxEXP <= CurrentEXP)
	{
		//레벨업
		float TempEXP = CurrentEXP - MaxEXP;
		SetNewLevel(Level + 1);
		CurrentEXP = TempEXP;
		OnLevelUpDelegate.Broadcast();
		//레벨업하고도 레벨업할 경험치가 된다면 한 번 더 이 함수를 실행함
		if (MaxEXP <= CurrentEXP) SetEXP(0);
	}
	OnEXPChangedDelegate.Broadcast();
}

//Get
float UPA_Player_PlayerStat::GetHPRatio() const
{
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
}
float UPA_Player_PlayerStat::GetMPRatio() const
{
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentMP / MaxMP);
}
float UPA_Player_PlayerStat::GetEXPRatio() const
{
	return (CurrentEXP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentEXP / MaxEXP);
}

bool UPA_Player_PlayerStat::ManaCheck(float UsedMana)
{
	if (CurrentMP >= UsedMana) return true;
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Enough Mana"));
		return false;
	}
}