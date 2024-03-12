// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_UI.h"
#include "PA_Icon_Widget.h"
#include "PA_Setting.h"
#include "PA_Player_PlayerStat.h"

#include "Components/TextBlock.h"			//텍스트
#include "Components/ProgressBar.h"			//프로그레스 바


void UPA_Player_UI::NativeConstruct()
{
	Super::NativeConstruct();

	//프로그래스바
	ProgressBar_HP = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar_HP")));		//HP 프로그레스 바
	ProgressBar_MP = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar_MP")));		//MP 프로그레스 바
	ProgressBar_EXP = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar_EXP")));		//EXP 프로그레스 바
	//텍스트
	TextBlock_HP = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_HP")));				//HP 텍스트
	TextBlock_MP = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_MP")));				//MP 텍스트
	TextBlock_EXP = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_EXP")));			//EXP 텍스트
	//슬롯
	WBP_SkillSlotQ = Cast<UPA_Icon_Widget>(GetWidgetFromName(FName("WBP_SkillSlotQ")));			//스킬 슬롯1
	WBP_SkillSlotE = Cast<UPA_Icon_Widget>(GetWidgetFromName(FName("WBP_SkillSlotE")));			//스킬 슬롯2
	WBP_SkillSlotR = Cast<UPA_Icon_Widget>(GetWidgetFromName(FName("WBP_SkillSlotR")));			//스킬 슬롯3
	WBP_SkillSlotG = Cast<UPA_Icon_Widget>(GetWidgetFromName(FName("WBP_SkillSlotG")));			//스킬 슬롯4
	WBP_SkillSlotShift = Cast<UPA_Icon_Widget>(GetWidgetFromName(FName("WBP_SkillSlotShift")));	//스킬 슬롯5

	//프로그래스 바와 텍스트를 초기화
	UpdateHPBar();
	UpdateMPBar();
	UpdateEXPBar();
	SetHPText();
	SetMPText();
	SetEXPText();
}

//ProgressBar Update
void UPA_Player_UI::UpdateHPBar()
{
	if (CurrentPlayerStat.IsValid()) if (ProgressBar_HP != nullptr) ProgressBar_HP->SetPercent(CurrentPlayerStat->GetHPRatio());
}
void UPA_Player_UI::UpdateMPBar()
{
	if (CurrentPlayerStat.IsValid()) if (ProgressBar_MP != nullptr) ProgressBar_MP->SetPercent(CurrentPlayerStat->GetMPRatio());
}
void UPA_Player_UI::UpdateEXPBar()
{
	if (CurrentPlayerStat.IsValid()) if (ProgressBar_EXP != nullptr) ProgressBar_EXP->SetPercent(CurrentPlayerStat->GetEXPRatio());
}

//Text Update
void UPA_Player_UI::SetHPText()
{
	if (CurrentPlayerStat.IsValid())
	{
		int32 iCurrentHP = static_cast<int32>(CurrentPlayerStat->GetCurrentHP());
		int32 iMaxHP = static_cast<int32>(CurrentPlayerStat->GetMaxHP());
		if (TextBlock_HP != nullptr) TextBlock_HP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), iCurrentHP, iMaxHP)));
	}
}
void UPA_Player_UI::SetMPText()
{
	if (CurrentPlayerStat.IsValid())
	{
		int32 iCurrentMP = static_cast<int32>(CurrentPlayerStat->GetCurrentMP());
		int32 iMaxMP = static_cast<int32>(CurrentPlayerStat->GetMaxMP());
		if (TextBlock_MP != nullptr) TextBlock_MP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), iCurrentMP, iMaxMP)));
	}
}
void UPA_Player_UI::SetEXPText()
{
	if (CurrentPlayerStat.IsValid())
	{
		int32 iCurrentEXP = static_cast<int32>(CurrentPlayerStat->GetCurrentEXP());
		int32 iMaxEXP = static_cast<int32>(CurrentPlayerStat->GetMaxEXP());
		if (TextBlock_EXP != nullptr) TextBlock_EXP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), iCurrentEXP, iMaxEXP)));
	}
}

//Active Skill
void UPA_Player_UI::ActiveSkillSlotQ()
{
	WBP_SkillSlotQ->ActiveSkill();
}
void UPA_Player_UI::ActiveSkillSlotE()
{
	WBP_SkillSlotE->ActiveSkill();
}
void UPA_Player_UI::ActiveSkillSlotR()
{
	WBP_SkillSlotR->ActiveSkill();
}
void UPA_Player_UI::ActiveSkillSlotG()
{
	WBP_SkillSlotG->ActiveSkill();
}
void UPA_Player_UI::ActiveSkillSlotShift()
{
	WBP_SkillSlotShift->ActiveSkill();
}
const bool UPA_Player_UI::GetIsActiveQ()
{
	return WBP_SkillSlotQ->IsActive;
}
const bool UPA_Player_UI::GetIsActiveE()
{
	return WBP_SkillSlotE->IsActive;
}
const bool UPA_Player_UI::GetIsActiveR()
{
	return WBP_SkillSlotR->IsActive;
}
const bool UPA_Player_UI::GetIsActiveG()
{
	return WBP_SkillSlotG->IsActive;
}
const bool UPA_Player_UI::GetIsActiveShift()
{
	return WBP_SkillSlotShift->IsActive;
}

//Set SkillCoolTime
void UPA_Player_UI::SetSkillCoolTime(int32 NewLevel)
{
	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();

	CoolTimeQ = DefaultSetting->GetBoomSkillData(NewLevel)->CoolTime;
	CoolTimeE = DefaultSetting->GetBlockSkillData(NewLevel)->CoolTime;
	CoolTimeR = DefaultSetting->GetArrowRainSkillData(NewLevel)->CoolTime;
	CoolTimeG = DefaultSetting->GetTeleportSkillData(NewLevel)->CoolTime;
	CoolTimeShift = DefaultSetting->GetDashSkillData(NewLevel)->CoolTime;
	ManaQ = DefaultSetting->GetDashSkillData(NewLevel)->Mana;
	ManaE = DefaultSetting->GetBlockSkillData(NewLevel)->Mana;
	ManaR = DefaultSetting->GetArrowRainSkillData(NewLevel)->Mana;
	ManaG = DefaultSetting->GetTeleportSkillData(NewLevel)->Mana;
	ManaShift = DefaultSetting->GetDashSkillData(NewLevel)->Mana;

	//스킬 슬롯 설정
	WBP_SkillSlotQ->SetSkill(ESkill::Boom, CoolTimeQ);
	WBP_SkillSlotE->SetSkill(ESkill::Block, CoolTimeE);
	WBP_SkillSlotR->SetSkill(ESkill::ArrowRain, CoolTimeR);
	WBP_SkillSlotG->SetSkill(ESkill::Teleport, CoolTimeG);
	WBP_SkillSlotShift->SetSkill(ESkill::Dash, CoolTimeShift);
}

//Bind Status
void UPA_Player_UI::BindPlayerStat(UPA_Player_PlayerStat* NewPlayerStat)
{
	if (NewPlayerStat != nullptr)
	{
		//델리게이트 설정
		CurrentPlayerStat = NewPlayerStat;
		NewPlayerStat->OnHPChangedDelegate.AddUObject(this, &UPA_Player_UI::UpdateHPBar);
		NewPlayerStat->OnHPChangedDelegate.AddUObject(this, &UPA_Player_UI::SetHPText);
		NewPlayerStat->OnMPChangedDelegate.AddUObject(this, &UPA_Player_UI::UpdateMPBar);
		NewPlayerStat->OnMPChangedDelegate.AddUObject(this, &UPA_Player_UI::SetMPText);
		NewPlayerStat->OnEXPChangedDelegate.AddUObject(this, &UPA_Player_UI::UpdateEXPBar);
		NewPlayerStat->OnEXPChangedDelegate.AddUObject(this, &UPA_Player_UI::SetEXPText);
		SetSkillCoolTime(NewPlayerStat->GetLevel());
	}
}