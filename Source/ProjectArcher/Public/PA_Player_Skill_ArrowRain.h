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

	//프로퍼티
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float Mana;				//사용될 마나
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float Damage;			//대미지
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "ArrowRain")
	float Coefficient;		//계수

	//Set 함수
		//레벨을 이용한 프로퍼티 설정
	void SetArrowRainProperty(int32 NewLevel);
		//플레이어의 공격력을 얻어오는 함수
	void SetArrowRainDamage(float PlayerAttackDamage);

private:
	struct FArrowRainSkillDataTable* CurrentPropertyData = nullptr;

	//대미지 ( 계수 )
	float CoefficientDamage = 0.0f;

	UFUNCTION(BlueprintCallable)
	void DoDamage(AActor* TakeDamagedActor);

};
