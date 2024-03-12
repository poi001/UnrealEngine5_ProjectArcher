// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_Skill_Teleport.h"
#include "PA_Setting.h"

#include "GameFramework/ProjectileMovementComponent.h"	//�߻�ü�����Ʈ, �����
#include "Components/SphereComponent.h"					//�����ݸ���

// Sets default values
APA_Player_Skill_Teleport::APA_Player_Skill_Teleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������Ʈ
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILEMOVEMENT"));
}

// Called when the game starts or when spawned
void APA_Player_Skill_Teleport::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APA_Player_Skill_Teleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DurationTime += DeltaTime;
	if (DurationTime >= 5.0f)
	{
		OnDestroy.Broadcast();
		Destroy();
	}
}

void APA_Player_Skill_Teleport::SetSkill(float NewLevel)
{
	//���ð��� ������Ʈ�� �����Ѵ�
	auto DefaultSetting = GetDefault<UPA_Setting>();
	
	//��ų������
	SkillDataTable = DefaultSetting->GetTeleportSkillData(NewLevel);

	//�ӵ� ����
	ProjectileMovement->Velocity = GetActorForwardVector() * SkillDataTable->Speed;
}