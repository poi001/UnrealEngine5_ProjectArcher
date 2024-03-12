// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"			//������ ���̺� ������� �߰�
#include "PA_Setting.generated.h"

/**
 * 
 */

//�÷��̾� ��ų Enum
UENUM(BlueprintType)
enum class ESkill : uint8
{
	Boom,
	Dash,
	Block,
	Teleport,
	ArrowRain
};

//Boom��ų ������
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FBoomSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FBoomSkillDataTable() :Level(1), CoolTime(15.0f), Mana(30.0f), Damage(50.0f), Coefficient(0.4f){}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	int32 Level;			//��ų ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float CoolTime;			//���� ���ð�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float Mana;				//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float Damage;			//�����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float Coefficient;		//���ݷ� ���
};

//Dash��ų ������
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FDashSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FDashSkillDataTable() :Level(1), CoolTime(10.0f), Mana(30.0f), Damage(0.0f), Coefficient(0.0f), Speed(3000.0f){}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	int32 Level;			//��ų ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float CoolTime;			//���� ���ð�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Mana;				//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Damage;			//�����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Coefficient;		//���ݷ� ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Speed;			//��� ���ǵ�
};

//Block��ų ������
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FBlockSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FBlockSkillDataTable() :Level(1), CoolTime(20.0f), Mana(30.0f), Damage(0.0f), Coefficient(0.0f), Extent(200.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	int32 Level;			//��ų ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float CoolTime;			//���� ���ð�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Mana;				//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Damage;			//�����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Coefficient;		//���ݷ� ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Extent;			//ũ��
};

//Teleport��ų ������
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FTeleportSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FTeleportSkillDataTable() :Level(1), CoolTime(10.0f), Mana(52.0f), Damage(0.0f), Coefficient(0.0f), Speed(600.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	int32 Level;			//��ų ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float CoolTime;			//���� ���ð�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Mana;				//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Damage;			//�����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Coefficient;		//���ݷ� ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Speed;			//ũ��
};

//ArrowRain��ų ������
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FArrowRainSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FArrowRainSkillDataTable() :Level(1), CoolTime(30.0f), Mana(90.0f), Damage(30.0f), Coefficient(1.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	int32 Level;			//��ų ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float CoolTime;			//���� ���ð�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float Mana;				//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float Damage;			//�����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float Coefficient;		//���ݷ� ���
};

//�÷��̾� ����
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FPlayerStatDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FPlayerStatDataTable() : Level(1), HP(200.0f), HPRegen(1.0f), MP(100.0f), MPRegen(2.0f), AttackDamage(30.0f), AttackSpeed(1.0f), 
		Exp(50.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	int32 Level;			//����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float HP;				//�ִ� ü��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float HPRegen;			//HP����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float MP;				//�ִ� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float MPRegen;			//MP����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float AttackDamage;		//���ݷ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float AttackSpeed;		//���ݼӵ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float Exp;				//�ִ� ����ġ��
};

//�� ����
USTRUCT(BlueprintType)	//�� ����ü�� �������Ʈ���� ������ ����� �� �ִ� �������� �����ŵ�ϴ�.
struct FEnemyStatDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FEnemyStatDataTable() : Code(0), Name("Gobline"), HP(200.0f), Speed(300.0f), AttackDamage(30.0f), AttackRange(1.0f),
		Exp(50.0f), Gold(0) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	int32 Code;				//������ �ڵ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	FName Name;				//�̸�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float HP;				//�ִ� ü��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float Speed;			//�̵� �ӵ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float AttackDamage;		//���ݷ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float AttackRange;		//���� ���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float Exp;				//������ ����ġ
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	int32 Gold;				//������ ���
};

UCLASS(config=Skill)
class PROJECTARCHER_API UPA_Setting : public UObject
{
	GENERATED_BODY()

public:
	UPA_Setting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> Skill_Icon;			//��ų ������ ���� ��θ� ���� �迭
	UPROPERTY(config)
	TArray<FSoftObjectPath> Skill_DataTable;	//��ų ���̺����� ���� ��θ� ���� �迭

	//Boom��ų ������ ���̺� Get�Լ�
	FBoomSkillDataTable* GetBoomSkillData(int32 Level) const;
	//Dash��ų ������ ���̺� Get�Լ�
	FDashSkillDataTable* GetDashSkillData(int32 Level) const;
	//Block��ų ������ ���̺� Get�Լ�
	FBlockSkillDataTable* GetBlockSkillData(int32 Level) const;
	//Teleport��ų ������ ���̺� Get�Լ�
	FTeleportSkillDataTable* GetTeleportSkillData(int32 Level) const;
	//ArrowRain��ų ������ ���̺� Get�Լ�
	FArrowRainSkillDataTable* GetArrowRainSkillData(int32 Level) const;

	//PlayerStatData Get�Լ�
	FPlayerStatDataTable* GetPlayerStatData(int32 Level) const;
	//EnemyStatData Get�Լ�
	FEnemyStatDataTable* GetEnemyStatData(int32 Code) const;

private:
	//Boom��ų ������ ���̺�
	UPROPERTY()
	class UDataTable* Skill_BoomDataTable;
	//Dash��ų ������ ���̺�
	UPROPERTY()
	class UDataTable* Skill_DashDataTable;
	//Block��ų ������ ���̺�
	UPROPERTY()
	class UDataTable* Skill_BlockDataTable;
	//Teleport��ų ������ ���̺�
	UPROPERTY()
	class UDataTable* Skill_TeleportDataTable;
	//ArrowRain��ų ������ ���̺�
	UPROPERTY()
	class UDataTable* Skill_ArrowRainDataTable;

	//PlayerStat ������ ���̺�
	UPROPERTY()
	class UDataTable* PlayerStatDataTable;
	//EnemyStat ������ ���̺�
	UPROPERTY()
	class UDataTable* EnemyStatDataTable;

};
