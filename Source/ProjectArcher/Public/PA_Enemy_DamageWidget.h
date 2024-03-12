// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PA_Enemy_DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Enemy_DamageWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	//����� �ؽ�Ʈ
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_DamageText;

	//����� ���� �ִϸ��̼�
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Animation", Meta = (BindWidgetAnim))	// Transient, Meta = (BindWidgetAnim) �ʼ�
	class UWidgetAnimation* NormalDamageAnimation;			//�Ϲ� ����� ���� �ִϸ��̼�
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Animation", Meta = (BindWidgetAnim))	// Transient, Meta = (BindWidgetAnim) �ʼ�
	class UWidgetAnimation* CriticalDamageAnimation;		//ġ��Ÿ ����� ���� �ִϸ��̼�

public:
	void PlayDamageAnimation(float Damage, bool IsCritical);
	void SetText(float Damage);
};
