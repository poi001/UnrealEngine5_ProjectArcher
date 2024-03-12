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

	//��������Ʈ
	FOnHPIsZeroDelegate OnHPIsZeroDelegate;		//ü���� 0�� ��
	FOnHPChangedDelegate OnHPChangedDelegate;	//ü���� ������ ��
	FOnMPChangedDelegate OnMPChangedDelegate;	//������ ������ ��
	FOnEXPChangedDelegate OnEXPChangedDelegate;	//����ġ�� ������ ��
	FOnLevelUpDelegate OnLevelUpDelegate;		//���������� ��

	//Set �Լ�
	void SetNewLevel(int32 NewLevel);							//������ �̿��� ���� ����
	void SetDamage(float NewDamage);							//������� ���� ��, ü�� ����
	void UseMana(float UsedMana);								//������ ����� ��, ȣ���ϴ� �Լ�
	void SetHP(float NewHP);									//���� ü�� ����
	void SetMP(float NewMP);									//���� ���� ����
	void SetEXP(float AddEXP);									//���� ����ġ ����

	//Get �Լ�
	float GetHPRatio() const;									//HP ���
	float GetMPRatio() const;									//MP ���
	float GetEXPRatio() const;									//EXP ���
	float GetLevel() const { return Level; }					//����
	float GetMaxHP() const { return MaxHP; }					//�ִ� ü��
	float GetCurrentHP() const { return CurrentHP; }			//���� ü��
	float GetMaxMP() const { return MaxMP; }					//�ִ� ����
	float GetCurrentMP() const { return CurrentMP; }			//���� ����
	float GetSpeed() const { return Speed; }					//�̵� �ӵ�
	float GetAttackDamage() const { return AttackDamage; }		//���� �����
	float GetAttackSpeed() const { return AttackSpeed; }		//���� �ӵ�
	float GetMaxEXP() const { return MaxEXP; }					//�ִ� ����ġ��
	float GetCurrentEXP() const { return CurrentEXP; }			//���� ����ġ��

	//temp
	bool ManaCheck(float UsedMana);


private:
	struct FPlayerStatDataTable* CurrentStatData = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int32 Level;		//����
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MaxHP;		//�ִ� ü��
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentHP;	//���� ü��
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float HPRegen;		//HP����
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MaxMP;		//�ִ� ����
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentMP;	//���� ����
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MPRegen;		//MP����
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float Speed;		//�̵� �ӵ�
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float AttackDamage;	//���� ������
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float AttackSpeed;	//���� �ӵ�
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float MaxEXP;		//�ִ� ����ġ��
	UPROPERTY(Transient, BlueprintReadOnly, EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float CurrentEXP;	//���� ����ġ��

	float TickBasket = 0.0f;
};
