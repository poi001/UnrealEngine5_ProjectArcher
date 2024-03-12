// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PA_Player_PlayerStat.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEXPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelUpDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTARCHER_API UPA_Player_PlayerStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPA_Player_PlayerStat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//델리게이트
	FOnHPIsZeroDelegate OnHPIsZeroDelegate;		//체력이 0일 때
	FOnHPChangedDelegate OnHPChangedDelegate;	//체력이 변했을 때
	FOnMPChangedDelegate OnMPChangedDelegate;	//마나가 변했을 때
	FOnEXPChangedDelegate OnEXPChangedDelegate;	//경험치가 변했을 때
	FOnLevelUpDelegate OnLevelUpDelegate;		//레벨업했을 때

	//Set 함수
	void SetNewLevel(int32 NewLevel);							//레벨을 이용한 스탯 설정
	void SetDamage(float NewDamage);							//대미지를 받을 때, 체력 설정
	void UseMana(float UsedMana);								//마나를 사용할 때, 호출하는 함수
	void SetHP(float NewHP);									//현재 체력 설정
	void SetMP(float NewMP);									//현재 마나 설정
	void SetEXP(float AddEXP);									//현재 경험치 설정

	//Get 함수
	float GetHPRatio() const;									//HP 비울
	float GetMPRatio() const;									//MP 비울
	float GetEXPRatio() const;									//EXP 비울
	float GetLevel() const { return Level; }					//레벨
	float GetMaxHP() const { return MaxHP; }					//최대 체력
	float GetCurrentHP() const { return CurrentHP; }			//현재 체력
	float GetMaxMP() const { return MaxMP; }					//최대 마나
	float GetCurrentMP() const { return CurrentMP; }			//현재 마나
	float GetSpeed() const { return Speed; }					//이동 속도
	float GetAttackDamage() const { return AttackDamage; }		//공격 대미지
	float GetAttackSpeed() const { return AttackSpeed; }		//공격 속도
	float GetMaxEXP() const { return MaxEXP; }					//최대 경험치량
	float GetCurrentEXP() const { return CurrentEXP; }			//현재 경험치량

	//temp
	bool ManaCheck(float UsedMana);


private:
	struct FPlayerStatDataTable* CurrentStatData = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 Level;		//레벨
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MaxHP;		//최대 체력
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentHP;	//현재 체력
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float HPRegen;		//HP리젠
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MaxMP;		//최대 마나
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentMP;	//현재 마나
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MPRegen;		//MP리젠
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float Speed;		//이동 속도
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float AttackDamage;	//공격 데미지
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float AttackSpeed;	//공격 속도
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MaxEXP;		//최대 경험치량
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentEXP;	//현재 경험치량

	float TickBasket = 0.0f;
};
