// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_PlayerController.h"
#include "PA_Player_PlayerCharacter.h"
#include "PA_Player_PlayerStat.h"


APA_Player_PlayerController::APA_Player_PlayerController()
{
	
}

void APA_Player_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	//플레이어 캐릭터 설정
	PlayerCharacter = Cast<APA_Player_PlayerCharacter>(aPawn);
	//플레이어 스탯 설정
	PlayerStat = Cast<UPA_Player_PlayerStat>(PlayerCharacter->CharacterStat);
}

void APA_Player_PlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void APA_Player_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}

void APA_Player_PlayerController::KillEnemy(float fEnemyEXP)
{
	if(PlayerStat != nullptr) PlayerStat->SetEXP(fEnemyEXP);
}