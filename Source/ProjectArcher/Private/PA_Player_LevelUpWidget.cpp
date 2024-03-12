// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_LevelUpWidget.h"

#include "Components/TextBlock.h"			//텍스트
#include "Animation/WidgetAnimation.h"		//위젯 애니메이션


void UPA_Player_LevelUpWidget::PlayLevelUpWidgetAnimation()
{
	//애니메이션 재생
	PlayAnimation(LevelUpWidgetAnimation);

	//1초 후에 위젯애니메이션을 Destroy하는 구문은 블루프린트에서 구현하였다
}