// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectArcherGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API AProjectArcherGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AProjectArcherGameModeBase();

	virtual void PostLogin(APlayerController* NewController) override;
};
