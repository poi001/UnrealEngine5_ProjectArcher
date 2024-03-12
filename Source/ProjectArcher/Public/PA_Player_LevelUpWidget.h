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
	//����� ���� �ִϸ��̼�
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Animation", Meta = (BindWidgetAnim))	// Transient, Meta = (BindWidgetAnim) �ʼ�
	class UWidgetAnimation* LevelUpWidgetAnimation;			//�Ϲ� ����� ���� �ִϸ��̼�

public:
	void PlayLevelUpWidgetAnimation();
};
