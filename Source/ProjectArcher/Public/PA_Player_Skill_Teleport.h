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

	//��������Ʈ
	FOnDestroy OnDestroy;				//���Ͱ� ����� ��, ��������Ʈ

	//��ų ������ ���̺�
	struct FTeleportSkillDataTable* SkillDataTable;

	//Set
	void SetSkill(float NewLevel);

private:
	//������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile", Meta = (AllowPrivateAccess = true))
	class UProjectileMovementComponent* ProjectileMovement;		//�߻�ü �����Ʈ

	//��ų�� ���ӵǴ� �ð�
	float DurationTime = 0.0f;
};
