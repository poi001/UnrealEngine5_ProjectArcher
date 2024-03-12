// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"	//FStreamableManager 헤더파일
#include "PA_Game_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTARCHER_API UPA_Game_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPA_Game_GameInstance();

	FStreamableManager StreamableManager;		//비동기 에셋 로딩을 위한 변수
};
