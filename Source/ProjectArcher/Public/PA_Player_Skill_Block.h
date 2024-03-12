// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_Player_Skill_Block.generated.h"

UCLASS()
class PROJECTARCHER_API APA_Player_Skill_Block : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APA_Player_Skill_Block();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//������Ƽ
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Block")
	float CoolTime;			//���� ���ð�
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Block")
	float Mana;				//���� ����
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Block")
	float Extent;			//ũ��

	//��� ������ �Լ� ( ������ �������Ʈ���� ���� )
	UFUNCTION(BlueprintNativeEvent)
	void BlockRiseUp();
	virtual void BlockRiseUp_Implementation();

	//Set �Լ�
	void SetBlockProperty(int32 NewLevel);							//������ �̿��� ������Ƽ ����

private:
	struct FBlockSkillDataTable* CurrentPropertyData = nullptr;
};
