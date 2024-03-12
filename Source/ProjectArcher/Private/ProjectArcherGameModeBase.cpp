// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectArcherGameModeBase.h"
#include "PA_Player_PlayerCharacter.h"
#include "PA_Player_PlayerController.h"
#include "PA_Player_PlayerState.h"
#include "PA_Game_GameStateBase.h"


AProjectArcherGameModeBase::AProjectArcherGameModeBase()
{
	//기본 폰
	static ConstructorHelpers::FClassFinder<APA_Player_PlayerCharacter> BPPlayerCharacter(TEXT("/Game/Character/Player/BP/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (BPPlayerCharacter.Succeeded()) DefaultPawnClass = BPPlayerCharacter.Class;
	//플레이어 컨트롤러
	PlayerControllerClass = APA_Player_PlayerController::StaticClass();
	//플레이어 스테이트
	PlayerStateClass = APA_Player_PlayerState::StaticClass();
	//게임 스테이트
	GameStateClass = APA_Game_GameStateBase::StaticClass();
}

void AProjectArcherGameModeBase::PostLogin(APlayerController* NewController)
{
	Super::PostLogin(NewController);

	//PlayerState의 초기화 함수를 사용할 코드 구현
}