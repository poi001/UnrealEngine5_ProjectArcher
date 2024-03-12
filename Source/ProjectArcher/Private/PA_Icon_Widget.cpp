// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Icon_Widget.h"

#include "Components/Image.h"				//�̹���
#include "Components/TextBlock.h"			//�ؽ�Ʈ
#include "Components/ProgressBar.h"			//���α׷��� ��



void UPA_Icon_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	//��ų ������
	Image_Icon = Cast<UImage>(GetWidgetFromName(FName("Image_Icon")));
	if (Image_Icon == nullptr) UE_LOG(LogTemp, Error, TEXT("Player`s Image_Icon is nullptr"));

	//��Ÿ�� �ؽ�Ʈ
	TextBlock_CoolTime = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_CoolTime")));
	if (TextBlock_CoolTime == nullptr) UE_LOG(LogTemp, Error, TEXT("Player`s TextBlock_CoolTime is nullptr"));

	//��Ÿ�� ���α׷�����
	ProgressBar_CoolTime = Cast<UProgressBar>(GetWidgetFromName(FName("ProgressBar_CoolTime")));
	if (ProgressBar_CoolTime == nullptr) UE_LOG(LogTemp, Error, TEXT("Player`s ProgressBar_CoolTime is nullptr"));

	//������Ƽ �⺻ ����
	Skill = ESkill::Boom;
	RemainingTime = 0.0f;
}

void UPA_Icon_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//��ų ��Ÿ���� �� á�� ���� �� �Լ��� �۵����� �ʴ´�.
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

	//���ð��� ������Ʈ�� �����Ѵ�
	auto DefaultSetting = GetDefault<UPA_Setting>();

	//��ų ������ ��� �������ٰ� ������ ��ų �������� ��θ� �ִ´�
	int32 SkillIconNum = FMath::Clamp(static_cast<int32>(ESkill), 0, DefaultSetting->Skill_Icon.Num() - 1);
	SkillIconToLoad = DefaultSetting->Skill_Icon[SkillIconNum];

	//������ ����
	auto Icon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *SkillIconToLoad.ToString()));
	Image_Icon->SetBrushFromTexture(Icon);

	//���α׷����� �ؽ�Ʈ ����
	SetProgressBar();
}

void UPA_Icon_Widget::ActiveSkill()
{
	if (IsActive) RemainingTime = CoolTime;
}

void UPA_Icon_Widget::SetProgressBar()
{
	RemainingTime = FMath::Clamp(RemainingTime, 0.0f, CoolTime);

	//��Ÿ���� á���� �ؽ�Ʈ�� �����, ���� �������� ��쿡�� ���� �ð��� �ؽ�Ʈ�� ����
	if (RemainingTime < 0.01f) TextBlock_CoolTime->SetText(FText::FromString(TEXT("")));
	else
	{
		int32 iRemainingTime = static_cast<int32>(RemainingTime);
		FString sRemainingTime = FString::Printf(TEXT("%d"), iRemainingTime);
		TextBlock_CoolTime->SetText(FText::FromString(sRemainingTime));
	}

	//�������� ���α׷����ٸ� �����Ѵ�
	float Percent = (RemainingTime < 0.0f) ? 0.0f : (RemainingTime / CoolTime);
	ProgressBar_CoolTime->SetPercent(Percent);

}