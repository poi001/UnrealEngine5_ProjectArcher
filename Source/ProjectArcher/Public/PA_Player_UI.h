// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PA_Player_UI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Player_UI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	//HPBar
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;
	//MPBar
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* ProgressBar_MP;
	//EXPBar
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* ProgressBar_EXP;

	//HPText
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_HP;
	//MPText
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_MP;
	//EXPText
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_EXP;

	//SkillSlotQ
	UPROPERTY(Meta = (BindWidget))
	class UPA_Icon_Widget* WBP_SkillSlotQ;
	//SkillSlotE
	UPROPERTY(Meta = (BindWidget))
	class UPA_Icon_Widget* WBP_SkillSlotE;
	//SkillSlotR
	UPROPERTY(Meta = (BindWidget))
	class UPA_Icon_Widget* WBP_SkillSlotR;
	//SkillSlotG
	UPROPERTY(Meta = (BindWidget))
	class UPA_Icon_Widget* WBP_SkillSlotG;
	//SkillSlotShift
	UPROPERTY(Meta = (BindWidget))
	class UPA_Icon_Widget* WBP_SkillSlotShift;

public:
	//HP변경시 호출 함수
	void UpdateHPBar();
	//MP변경시 호출 함수
	void UpdateMPBar();
	//EXP변경시 호출 함수
	void UpdateEXPBar();

	//HPText 변경 함수
	void SetHPText();
	//MPText 변경 함수
	void SetMPText();
	//EXPText 변경 함수
	void SetEXPText();

	//스킬을 사용했을 시, 호출되는 함수들
	void ActiveSkillSlotQ();
	void ActiveSkillSlotE();
	void ActiveSkillSlotR();
	void ActiveSkillSlotG();
	void ActiveSkillSlotShift();
	const bool GetIsActiveQ();
	const bool GetIsActiveE();
	const bool GetIsActiveR();
	const bool GetIsActiveG();
	const bool GetIsActiveShift();

	void SetSkillCoolTime(int32 NewLevel);

	//플레이어 캐릭터 스탯을 바인드하는 함수
	void BindPlayerStat(class UPA_Player_PlayerStat* NewPlayerStat);

	//스킬들의 마나소모량
	float ManaQ = 0.0f;
	float ManaE = 0.0f;
	float ManaR = 0.0f;
	float ManaG = 0.0f;
	float ManaShift = 0.0f;

private:
	TWeakObjectPtr<class UPA_Player_PlayerStat> CurrentPlayerStat;		//캐릭터 Status
	struct FPlayerStatDataTable* CurrentStatData = nullptr;

	//스킬들의 쿨타임들
	float CoolTimeQ = 0.0f;
	float CoolTimeE = 0.0f;
	float CoolTimeR = 0.0f;
	float CoolTimeG = 0.0f;
	float CoolTimeShift = 0.0f;
};
