// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PA_Player_LevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Player_LevelUpWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//대미지 위젯 애니메이션
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Animation", Meta = (BindWidgetAnim))	// Transient, Meta = (BindWidgetAnim) 필수
	class UWidgetAnimation* LevelUpWidgetAnimation;			//일반 대미지 위젯 애니메이션

public:
	void PlayLevelUpWidgetAnimation();
};
