// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectArcherGameModeBase.h"
#include "PA_Player_PlayerCharacter.h"
#include "PA_Player_PlayerController.h"
#include "PA_Player_PlayerState.h"
#include "PA_Game_GameStateBase.h"


AProjectArcherGameModeBase::AProjectArcherGameModeBase()
{
	//�⺻ ��
	static ConstructorHelpers::FClassFinder<APA_Player_PlayerCharacter> BPPlayerCharacter(TEXT("/Game/Character/Player/BP/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (BPPlayerCharacter.Succeeded()) DefaultPawnClass = BPPlayerCharacter.Class;
	//�÷��̾� ��Ʈ�ѷ�
	PlayerControllerClass = APA_Player_PlayerController::StaticClass();
	//�÷��̾� ������Ʈ
	PlayerStateClass = APA_Player_PlayerState::StaticClass();
	//���� ������Ʈ
	GameStateClass = APA_Game_GameStateBase::StaticClass();
}

void AProjectArcherGameModeBase::PostLogin(APlayerController* NewController)
{
	Super::PostLogin(NewController);

	//PlayerState�� �ʱ�ȭ �Լ��� ����� �ڵ� ����
}