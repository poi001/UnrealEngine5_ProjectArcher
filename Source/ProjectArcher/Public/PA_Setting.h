// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"			//데이터 테이블 헤더파일 추가
#include "PA_Setting.generated.h"

/**
 * 
 */

//플레이어 스킬 Enum
UENUM(BlueprintType)
enum class ESkill : uint8
{
	Boom,
	Dash,
	Block,
	Teleport,
	ArrowRain
};

//Boom스킬 데이터
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FBoomSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FBoomSkillDataTable() :Level(1), CoolTime(15.0f), Mana(30.0f), Damage(50.0f), Coefficient(0.4f){}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	int32 Level;			//스킬 레벨
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float Mana;				//사용될 마나
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float Damage;			//대미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Boom")
	float Coefficient;		//공격력 계수
};

//Dash스킬 데이터
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FDashSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FDashSkillDataTable() :Level(1), CoolTime(10.0f), Mana(30.0f), Damage(0.0f), Coefficient(0.0f), Speed(3000.0f){}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	int32 Level;			//스킬 레벨
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Mana;				//사용될 마나
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Damage;			//대미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Coefficient;		//공격력 계수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Dash")
	float Speed;			//대시 스피드
};

//Block스킬 데이터
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FBlockSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FBlockSkillDataTable() :Level(1), CoolTime(20.0f), Mana(30.0f), Damage(0.0f), Coefficient(0.0f), Extent(200.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	int32 Level;			//스킬 레벨
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Mana;				//사용될 마나
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Damage;			//대미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Coefficient;		//공격력 계수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Block")
	float Extent;			//크기
};

//Teleport스킬 데이터
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FTeleportSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FTeleportSkillDataTable() :Level(1), CoolTime(10.0f), Mana(52.0f), Damage(0.0f), Coefficient(0.0f), Speed(600.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	int32 Level;			//스킬 레벨
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Mana;				//사용될 마나
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Damage;			//대미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Coefficient;		//공격력 계수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_Teleport")
	float Speed;			//크기
};

//ArrowRain스킬 데이터
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FArrowRainSkillDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FArrowRainSkillDataTable() :Level(1), CoolTime(30.0f), Mana(90.0f), Damage(30.0f), Coefficient(1.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	int32 Level;			//스킬 레벨
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float CoolTime;			//재사용 대기시간
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float Mana;				//사용될 마나
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float Damage;			//대미지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Skill_ArrowRain")
	float Coefficient;		//공격력 계수
};

//플레이어 스탯
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FPlayerStatDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FPlayerStatDataTable() : Level(1), HP(200.0f), HPRegen(1.0f), MP(100.0f), MPRegen(2.0f), AttackDamage(30.0f), AttackSpeed(1.0f), 
		Exp(50.0f) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	int32 Level;			//레벨
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float HP;				//최대 체력
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float HPRegen;			//HP리젠
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float MP;				//최대 마나
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float MPRegen;			//MP리젠
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float AttackDamage;		//공격력
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float AttackSpeed;		//공격속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "PlayerStatDataTable")
	float Exp;				//최대 경험치량
};

//적 스탯
USTRUCT(BlueprintType)	//이 구조체를 블루프린트에서 변수로 사용할 수 있는 유형으로 노출시킵니다.
struct FEnemyStatDataTable :public FTableRowBase
{
	GENERATED_BODY()

public:
	FEnemyStatDataTable() : Code(0), Name("Gobline"), HP(200.0f), Speed(300.0f), AttackDamage(30.0f), AttackRange(1.0f),
		Exp(50.0f), Gold(0) {}

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	int32 Code;				//구분할 코드
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	FName Name;				//이름
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float HP;				//최대 체력
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float Speed;			//이동 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float AttackDamage;		//공격력
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float AttackRange;		//공격 가능 범위
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	float Exp;				//제공할 경험치
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "EnemyStatDataTable")
	int32 Gold;				//제공할 골드
};

UCLASS(config=Skill)
class PROJECTARCHER_API UPA_Setting : public UObject
{
	GENERATED_BODY()

public:
	UPA_Setting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> Skill_Icon;			//스킬 아이콘 저장 경로를 담은 배열
	UPROPERTY(config)
	TArray<FSoftObjectPath> Skill_DataTable;	//스킬 데이블데이터 저장 경로를 담을 배열

	//Boom스킬 데이터 테이블 Get함수
	FBoomSkillDataTable* GetBoomSkillData(int32 Level) const;
	//Dash스킬 데이터 테이블 Get함수
	FDashSkillDataTable* GetDashSkillData(int32 Level) const;
	//Block스킬 데이터 테이블 Get함수
	FBlockSkillDataTable* GetBlockSkillData(int32 Level) const;
	//Teleport스킬 데이터 테이블 Get함수
	FTeleportSkillDataTable* GetTeleportSkillData(int32 Level) const;
	//ArrowRain스킬 데이터 테이블 Get함수
	FArrowRainSkillDataTable* GetArrowRainSkillData(int32 Level) const;

	//PlayerStatData Get함수
	FPlayerStatDataTable* GetPlayerStatData(int32 Level) const;
	//EnemyStatData Get함수
	FEnemyStatDataTable* GetEnemyStatData(int32 Code) const;

private:
	//Boom스킬 데이터 테이블
	UPROPERTY()
	class UDataTable* Skill_BoomDataTable;
	//Dash스킬 데이터 테이블
	UPROPERTY()
	class UDataTable* Skill_DashDataTable;
	//Block스킬 데이터 테이블
	UPROPERTY()
	class UDataTable* Skill_BlockDataTable;
	//Teleport스킬 데이터 테이블
	UPROPERTY()
	class UDataTable* Skill_TeleportDataTable;
	//ArrowRain스킬 데이터 테이블
	UPROPERTY()
	class UDataTable* Skill_ArrowRainDataTable;

	//PlayerStat 데이터 테이블
	UPROPERTY()
	class UDataTable* PlayerStatDataTable;
	//EnemyStat 데이터 테이블
	UPROPERTY()
	class UDataTable* EnemyStatDataTable;

};
