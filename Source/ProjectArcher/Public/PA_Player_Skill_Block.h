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

	//프로퍼티
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Block")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Block")
	float Mana;				//사용될 마나
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Block")
	float Extent;			//크기

	//기둥 움직임 함수 ( 구현은 블루프린트에서 구현 )
	UFUNCTION(BlueprintNativeEvent)
	void BlockRiseUp();
	virtual void BlockRiseUp_Implementation();

	//Set 함수
	void SetBlockProperty(int32 NewLevel);							//레벨을 이용한 프로퍼티 설정

private:
	struct FBlockSkillDataTable* CurrentPropertyData = nullptr;
};
