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
	class UBehaviorTree* BTAsset;				//�����̾�Ʈ�� ����
	UPROPERTY()
	class UBlackboardData* BBAsset;				//������ ����


public:
	static const FName TargetKey;
	static const FName IsDetectedKey;
	static const FName AttackNumber;



	void RunAI();							//AI�۵�
	void StopAI();							//AI����

	//Set
	void SetIsDetectedKey(bool Set);			//IsDetectedKey����
	void SetGoblin();							//������, �����̾�Ʈ���� ������� ����
	void SetZombie();							//������, �����̾�Ʈ���� ����� ����
	void SetLich();								//������, �����̾�Ʈ���� ��ġ�� ����
	//Get
	const bool GetIsDetectedKey();					//IsDetectedKey��ȯ
	const int32 GetAttackNumber();					//AttackNumber��ȯ
};
