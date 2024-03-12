// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_Player_Skill_Boom.generated.h"

UCLASS()
class PROJECTARCHER_API APA_Player_Skill_Boom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APA_Player_Skill_Boom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//프로퍼티
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float Mana;				//사용될 마나
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float Damage;			//대미지
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float Coefficient;		//계수

	//폭탄 스피드
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Boom")
	float Speed = 2000.0f;
	//폭탄 범위 반지름
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Boom")
	float Radius = 22.5f;

	//Set 함수
	void SetBoomProperty(int32 NewLevel);							//레벨을 이용한 프로퍼티 설정

private:
	struct FBoomSkillDataTable* CurrentPropertyData = nullptr;
};
