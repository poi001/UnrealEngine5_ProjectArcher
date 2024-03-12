// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_DamageWidget.h"

#include "Components/TextBlock.h"			//�ؽ�Ʈ
#include "Animation/WidgetAnimation.h"		//���� �ִϸ��̼�

void UPA_Enemy_DamageWidget::NativeConstruct()
{
	//�ؽ�Ʈ
	TextBlock_DamageText = Cast<UTextBlock>(GetWidgetFromName(FName("TextBlock_DamageText")));
	if (TextBlock_DamageText == nullptr) UE_LOG(LogTemp, Error, TEXT("TextBlock_DamageText is nullptr"));
}

void UPA_Enemy_DamageWidget::PlayDamageAnimation(float Damage, bool IsCritical)
{
	//Text����
	SetText(Damage);

	//�⺻ ����� ������ ũ��Ƽ�� ����� ������ �����Ѵ�
	if (IsCritical) PlayAnimation(CriticalDamageAnimation);		//ġ��Ÿ ����� ���� �ִϸ��̼�
	else PlayAnimation(NormalDamageAnimation);					//�Ϲ� ����� ���� �ִϸ��̼�
}

void UPA_Enemy_DamageWidget::SetText(float Damage)
{
	int32 CastIntDamage = static_cast<int32>(Damage);
	TextBlock_DamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CastIntDamage)));
}