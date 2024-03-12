// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PA_Setting.h"
#include "Blueprint/UserWidget.h"
#include "PA_Icon_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Icon_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//스킬 아이콘
	UPROPERTY(Meta = (BindWidget))
	class UImage* Image_Icon;

	//쿨타임 텍스트
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_CoolTime;

	//쿨타임 프로그래스바
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* ProgressBar_CoolTime;

	//재사용 대기시간
	UPROPERTY()
	float CoolTime = 0.0f;

	//남은 시간
	UPROPERTY()
	float RemainingTime;

	//이 위젯의 스킬
	UPROPERTY()
	ESkill Skill;

public:
	//이 아이콘의 스킬을 반환함
	ESkill GetSkill();
	//재사용 대기시간 설정
	void SetSkill(ESkill ESkill, float fCoolTime);		//스킬을 초기화함

	//스킬을 사용했을 시
	void ActiveSkill();

	//쿨타임인지 확인용 변수
	bool IsActive = false;

private:
	FSoftObjectPath SkillIconToLoad = FSoftObjectPath(nullptr);				//스킬 아이콘 경로

	//아이콘 프로그래스바 설정
	void SetProgressBar();
};
