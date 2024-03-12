// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Icon_Widget.h"

#include "Components/Image.h"				//이미지
#include "Components/TextBlock.h"			//텍스트
#include "Components/ProgressBar.h"			//프로그레스 바



void UPA_Icon_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	//스킬 아이콘
	Image_Icon = Cast<UImage>(GetWidgetFromName(FName("Image_Icon")));
	if (Image_Icon == nullptr) UE_LOG(LogTemp, Error, TEXT("Player`s Image_Icon is nullptr"));

	//쿨타임 텍스트
	TextBlock_CoolTime = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_CoolTime")));
	if (TextBlock_CoolTime == nullptr) UE_LOG(LogTemp, Error, TEXT("Player`s TextBlock_CoolTime is nullptr"));

	//쿨타임 프로그래스바
	ProgressBar_CoolTime = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar_CoolTime")));
	if (ProgressBar_CoolTime == nullptr) UE_LOG(LogTemp, Error, TEXT("Player`s ProgressBar_CoolTime is nullptr"));

	//프로퍼티 기본 설정
	Skill = ESkill::Boom;
	RemainingTime = 0.0f;
}

void UPA_Icon_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//스킬 쿨타임이 다 찼을 때는 이 함수를 작동하지 않는다.
	if (RemainingTime > 0.0f)//KINDA_SMALL_NUMBER
	{
		RemainingTime -= InDeltaTime;
		IsActive = false;
		SetProgressBar();
	}
	else
	{
		IsActive = true;
	}
}

ESkill UPA_Icon_Widget::GetSkill()
{
	return Skill;
}

void UPA_Icon_Widget::SetSkill(ESkill ESkill, float fCoolTime)
{
	Skill = ESkill;
	CoolTime = FMath::Clamp(fCoolTime, 0.5f, 9999.0f);

	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();

	//스킬 아이콘 경로 변수에다가 설정한 스킬 아이콘의 경로를 넣는다
	int32 SkillIconNum = FMath::Clamp(static_cast<int32>(ESkill), 0, DefaultSetting->Skill_Icon.Num() - 1);
	SkillIconToLoad = DefaultSetting->Skill_Icon[SkillIconNum];

	//아이콘 설정
	auto Icon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *SkillIconToLoad.ToString()));
	Image_Icon->SetBrushFromTexture(Icon);

	//프로그래스바 텍스트 설정
	SetProgressBar();
}

void UPA_Icon_Widget::ActiveSkill()
{
	if (IsActive) RemainingTime = CoolTime;
}

void UPA_Icon_Widget::SetProgressBar()
{
	RemainingTime = FMath::Clamp(RemainingTime, 0.0f, CoolTime);

	//쿨타임이 찼으면 텍스트를 지우고, 아직 남아있을 경우에는 남은 시간을 텍스트로 띄운다
	if (RemainingTime < 0.01f) TextBlock_CoolTime->SetText(FText::FromString(TEXT("")));
	else
	{
		int32 iRemainingTime = static_cast<int32>(RemainingTime);
		FString sRemainingTime = FString::Printf(TEXT("%d"), iRemainingTime);
		TextBlock_CoolTime->SetText(FText::FromString(sRemainingTime));
	}

	//아이콘의 프로그래스바를 설정한다
	float Percent = (RemainingTime < 0.0f) ? 0.0f : (RemainingTime / CoolTime);
	ProgressBar_CoolTime->SetPercent(Percent);

}