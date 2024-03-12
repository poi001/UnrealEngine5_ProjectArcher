// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_Skill_Teleport.h"
#include "PA_Setting.h"

#include "GameFramework/ProjectileMovementComponent.h"	//발사체무브먼트, 대미지
#include "Components/SphereComponent.h"					//구형콜리전

// Sets default values
APA_Player_Skill_Teleport::APA_Player_Skill_Teleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트
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
	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();
	
	//스킬데이터
	SkillDataTable = DefaultSetting->GetTeleportSkillData(NewLevel);

	//속도 설정
	ProjectileMovement->Velocity = GetActorForwardVector() * SkillDataTable->Speed;
}