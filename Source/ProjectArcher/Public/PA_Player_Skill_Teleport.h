// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_Player_Skill_Teleport.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDestroy)

UCLASS()
class PROJECTARCHER_API APA_Player_Skill_Teleport : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APA_Player_Skill_Teleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//델리게이트
	FOnDestroy OnDestroy;				//액터가 사라질 때, 델리게이트

	//스킬 데이터 테이블
	struct FTeleportSkillDataTable* SkillDataTable;

	//Set
	void SetSkill(float NewLevel);

private:
	//컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", Meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* ProjectileMovement;		//발사체 무브먼트

	//스킬이 지속되는 시간
	float DurationTime = 0.0f;
};
