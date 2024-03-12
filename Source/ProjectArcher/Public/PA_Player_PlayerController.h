// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PA_Player_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API APA_Player_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APA_Player_PlayerController();

	virtual void OnPossess(APawn* aPawn) override;

	//적을 죽일 시
	void KillEnemy(float fEnemyEXP);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	//플레이어 캐릭터
	TWeakObjectPtr<class APA_Player_PlayerCharacter> PlayerCharacter;
	//플레이어 스탯
	TWeakObjectPtr<class UPA_Player_PlayerStat> PlayerStat;
};
