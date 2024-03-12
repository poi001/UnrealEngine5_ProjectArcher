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

	//��������Ʈ
	FOnHPIsZeroDelegate OnHPIsZeroDelegate;		//ü���� 0�� ��
	FOnHPChangedDelegate OnHPChangedDelegate;	//ü���� ������ ��

	//Set �Լ�
	void SetNewCode(int32 NewCode);							//�ڵ带 �̿��� ���� ����
	void SetDamage(float NewDamage);						//������� ���� ��, ü�� ����
	void SetHP(float NewHP);								//���� ü�� ����

	//Get �Լ�
	float GetHPRatio() const;									//HP ���
	float GetMaxHP() const { return MaxHP; }					//�ִ� ü��
	float GetCurrentHP() const { return CurrentHP; }			//���� ü��
	float GetSpeed() const { return Speed; }					//�̵� �ӵ�
	float GetAttackDamage() const { return AttackDamage; }		//���� �����
	float GetAttackRange() const { return AttackRange; }		//���� ����
	float GetEXP() const { return EXP; }						//�����ϴ� ����ġ��
	float GetGold() const { return Gold; }						//�����ϴ� ��差

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
