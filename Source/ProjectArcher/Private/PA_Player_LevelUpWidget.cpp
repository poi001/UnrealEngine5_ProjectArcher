// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_LevelUpWidget.h"

#include "Components/TextBlock.h"			//�ؽ�Ʈ
#include "Animation/WidgetAnimation.h"		//���� �ִϸ��̼�


void UPA_Player_LevelUpWidget::PlayLevelUpWidgetAnimation()
{
	//�ִϸ��̼� ���
	PlayAnimation(LevelUpWidgetAnimation);

	//1�� �Ŀ� �����ִϸ��̼��� Destroy�ϴ� ������ �������Ʈ���� �����Ͽ���
}