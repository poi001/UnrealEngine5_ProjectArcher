// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_DamageWidget.h"

#include "Components/TextBlock.h"			//텍스트
#include "Animation/WidgetAnimation.h"		//위젯 애니메이션

void UPA_Enemy_DamageWidget::NativeConstruct()
{
	//텍스트
	TextBlock_DamageText = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_DamageText")));
	if (TextBlock_DamageText == nullptr) UE_LOG(LogTemp, Error, TEXT("TextBlock_DamageText is nullptr"));
}

void UPA_Enemy_DamageWidget::PlayDamageAnimation(float Damage, bool IsCritical)
{
	//Text변경
	SetText(Damage);

	//기본 대미지 위젯과 크리티컬 대미지 위젯을 구분한다
	if (IsCritical) PlayAnimation(CriticalDamageAnimation);		//치명타 대미지 위젯 애니메이션
	else PlayAnimation(NormalDamageAnimation);					//일반 대미지 위젯 애니메이션
}

void UPA_Enemy_DamageWidget::SetText(float Damage)
{
	int32 CastIntDamage = static_cast<int32>(Damage);
	TextBlock_DamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CastIntDamage)));
}