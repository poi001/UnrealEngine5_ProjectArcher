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

	//대미지 텍스트
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_DamageText;

	//대미지 위젯 애니메이션
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Animation", Meta = (BindWidgetAnim))	// Transient, Meta = (BindWidgetAnim) 필수
	class UWidgetAnimation* NormalDamageAnimation;			//일반 대미지 위젯 애니메이션
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Animation", Meta = (BindWidgetAnim))	// Transient, Meta = (BindWidgetAnim) 필수
	class UWidgetAnimation* CriticalDamageAnimation;		//치명타 대미지 위젯 애니메이션

public:
	void PlayDamageAnimation(float Damage, bool IsCritical);
	void SetText(float Damage);
};
