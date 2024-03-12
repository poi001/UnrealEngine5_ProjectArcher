// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PA_Enemy_EnemyStat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTARCHER_API UPA_Enemy_EnemyStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPA_Enemy_EnemyStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//델리게이트
	FOnHPIsZeroDelegate OnHPIsZeroDelegate;		//체력이 0일 때
	FOnHPChangedDelegate OnHPChangedDelegate;	//체력이 변했을 때

	//Set 함수
	void SetNewCode(int32 NewCode);							//코드를 이용한 스탯 설정
	void SetDamage(float NewDamage);						//대미지를 받을 때, 체력 설정
	void SetHP(float NewHP);								//현재 체력 설정

	//Get 함수
	float GetHPRatio() const;									//HP 비울
	float GetMaxHP() const { return MaxHP; }					//최대 체력
	float GetCurrentHP() const { return CurrentHP; }			//현재 체력
	float GetSpeed() const { return Speed; }					//이동 속도
	float GetAttackDamage() const { return AttackDamage; }		//공격 대미지
	float GetAttackRange() const { return AttackRange; }		//공격 범위
	float GetEXP() const { return EXP; }						//제공하는 경험치량
	float GetGold() const { return Gold; }						//제공하는 골드량

private:
	struct FEnemyStatDataTable* CurrentStatData = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Code;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FName Name;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float Speed;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float AttackDamage;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float EXP;
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Gold;
};
