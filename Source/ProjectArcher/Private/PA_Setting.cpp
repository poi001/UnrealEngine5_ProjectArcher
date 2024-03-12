// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Setting.h"

UPA_Setting::UPA_Setting()
{
	//Boom
	FString BoomSkillDataPath = TEXT("/Script/Engine.DataTable'/Game/Character/Player/DataTable/Skill/SkillDataTable_Boom_csv.SkillDataTable_Boom_csv'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BOOMSKILL(*BoomSkillDataPath);
	if (DT_BOOMSKILL.Succeeded()) Skill_BoomDataTable = DT_BOOMSKILL.Object;
	//Dash
	FString DashSkillDataPath = TEXT("/Game/Character/Player/DataTable/Skill/SkillDataTable_Dash_csv.SkillDataTable_Dash_csv");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DASHSKILL(*DashSkillDataPath);
	if (DT_DASHSKILL.Succeeded()) Skill_DashDataTable = DT_DASHSKILL.Object;
	//Block
	FString BlockSkillDataPath = TEXT("/Game/Character/Player/DataTable/Skill/SkillDataTable_Block_csv.SkillDataTable_Block_csv");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BLOCKSKILL(*BlockSkillDataPath);
	if (DT_BLOCKSKILL.Succeeded()) Skill_BlockDataTable = DT_BLOCKSKILL.Object;
	//Teleport
	FString TeleportSkillDataPath = TEXT("/Game/Character/Player/DataTable/Skill/SkillDataTable_Teleport_csv.SkillDataTable_Teleport_csv");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TELEPORTSKILL(*TeleportSkillDataPath);
	if (DT_TELEPORTSKILL.Succeeded()) Skill_TeleportDataTable = DT_TELEPORTSKILL.Object;
	//ArrowRain
	FString ArrowRainSkillDataPath = TEXT("/Game/Character/Player/DataTable/Skill/SkillDataTable_ArrowRain_csv.SkillDataTable_ArrowRain_csv");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ARROWRAINSKILL(*ArrowRainSkillDataPath);
	if (DT_ARROWRAINSKILL.Succeeded()) Skill_ArrowRainDataTable = DT_ARROWRAINSKILL.Object;

	//플레이어 스탯
	FString PlayerStatDataPath = TEXT("/Game/Character/Player/DataTable/Status/PlayerDataTable_csv.PlayerDataTable_csv");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PLAYERSTAT(*PlayerStatDataPath);
	if (DT_PLAYERSTAT.Succeeded()) PlayerStatDataTable = DT_PLAYERSTAT.Object;
	//적 스탯
	FString EnemyStatDataPath = TEXT("/Game/Character/Enemy/DataTable/EnemyDataTable_csv.EnemyDataTable_csv");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMYSTAT(*EnemyStatDataPath);
	if (DT_ENEMYSTAT.Succeeded()) EnemyStatDataTable = DT_ENEMYSTAT.Object;
}

//Get 함수
FBoomSkillDataTable* UPA_Setting::GetBoomSkillData(int32 Level) const
{
	Level = FMath::Clamp(Level, 1, 20);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return Skill_BoomDataTable->FindRow<FBoomSkillDataTable>(*FString::FromInt(Level), TEXT(""));
}
FDashSkillDataTable* UPA_Setting::GetDashSkillData(int32 Level) const
{
	Level = FMath::Clamp(Level, 1, 20);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return Skill_DashDataTable->FindRow<FDashSkillDataTable>(*FString::FromInt(Level), TEXT(""));
}
FBlockSkillDataTable* UPA_Setting::GetBlockSkillData(int32 Level) const
{
	Level = FMath::Clamp(Level, 1, 20);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return Skill_BlockDataTable->FindRow<FBlockSkillDataTable>(*FString::FromInt(Level), TEXT(""));
}
FTeleportSkillDataTable* UPA_Setting::GetTeleportSkillData(int32 Level) const
{
	Level = FMath::Clamp(Level, 1, 20);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return Skill_TeleportDataTable->FindRow<FTeleportSkillDataTable>(*FString::FromInt(Level), TEXT(""));
}
FArrowRainSkillDataTable* UPA_Setting::GetArrowRainSkillData(int32 Level) const
{
	Level = FMath::Clamp(Level, 1, 20);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return Skill_ArrowRainDataTable->FindRow<FArrowRainSkillDataTable>(*FString::FromInt(Level), TEXT(""));
}
FPlayerStatDataTable* UPA_Setting::GetPlayerStatData(int32 Level) const
{
	Level = FMath::Clamp(Level, 1, 20);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return PlayerStatDataTable->FindRow<FPlayerStatDataTable>(*FString::FromInt(Level), TEXT(""));
}
FEnemyStatDataTable* UPA_Setting::GetEnemyStatData(int32 Code) const
{
	Code = FMath::Clamp(Code, 0, 999);

	//TEXT("")는 검색을 제대로 하지 못했을 경우 이것을 출력하라는 인자이다
	return EnemyStatDataTable->FindRow<FEnemyStatDataTable>(*FString::FromInt(Code), TEXT(""));
}