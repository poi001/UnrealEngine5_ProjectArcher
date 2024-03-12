// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PA_Enemy_Character.h"
#include "PA_Enemy_Character_Goblin.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API APA_Enemy_Character_Goblin : public APA_Enemy_Character
{
	GENERATED_BODY()
	
public:
	APA_Enemy_Character_Goblin();

	//�ʱ�ȭ ����
	virtual void PostInitializeComponents() override;
	//�� ���Ͱ� ��Ʈ�ѷ��� �������� ���� ȣ�� �Լ�
	virtual void PossessedBy(AController* NewController) override;

private:
	virtual void BeginPlay() override;

	virtual void AttackCheck() override;
	virtual void AttackEnd() override;

	//���� ���� ������
	float AttackRadius = 50.0f;
};
