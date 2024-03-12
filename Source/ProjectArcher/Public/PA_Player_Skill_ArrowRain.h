// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_Player_Skill_ArrowRain.generated.h"

UCLASS()
class PROJECTARCHER_API APA_Player_Skill_ArrowRain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APA_Player_Skill_ArrowRain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//������Ƽ
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float CoolTime;			//���� ���ð�
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float Mana;				//���� ����
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float Damage;			//�����
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float Coefficient;		//���

	//Set �Լ�
		//������ �̿��� ������Ƽ ����
	void SetArrowRainProperty(int32 NewLevel);
		//�÷��̾��� ���ݷ��� ������ �Լ�
	void SetArrowRainDamage(float PlayerAttackDamage);

private:
	struct FArrowRainSkillDataTable* CurrentPropertyData = nullptr;

	//����� ( ��� )
	float CoefficientDamage = 0.0f;

	UFUNCTION(BlueprintCallable)
	void DoDamage(AActor* TakeDamagedActor);

};
