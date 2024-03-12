// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PA_Enemy_Controller.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API APA_Enemy_Controller : public AAIController
{
	GENERATED_BODY()
	
public:
	APA_Enemy_Controller();

protected:
	UPROPERTY()
	class UBehaviorTree* BTAsset;				//비헤이어트리 에셋
	UPROPERTY()
	class UBlackboardData* BBAsset;				//블랙보드 에셋


public:
	static const FName TargetKey;
	static const FName IsDetectedKey;
	static const FName AttackNumber;



	void RunAI();							//AI작동
	void StopAI();							//AI정지

	//Set
	void SetIsDetectedKey(bool Set);			//IsDetectedKey변경
	void SetGoblin();							//블랙보드, 비헤이어트리를 고블린으로 설정
	void SetZombie();							//블랙보드, 비헤이어트리를 좀비로 설정
	void SetLich();								//블랙보드, 비헤이어트리를 리치로 설정
	//Get
	const bool GetIsDetectedKey();					//IsDetectedKey반환
	const int32 GetAttackNumber();					//AttackNumber반환
};
