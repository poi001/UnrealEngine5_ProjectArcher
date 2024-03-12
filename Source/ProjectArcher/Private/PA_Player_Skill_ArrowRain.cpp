// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_Skill_ArrowRain.h"
#include "PA_Setting.h"
#include "PA_Enemy_Character.h"
#include "PA_Player_PlayerController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APA_Player_Skill_ArrowRain::APA_Player_Skill_ArrowRain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//프로퍼티 초기화
	SetArrowRainProperty(1);
}

// Called when the game starts or when spawned
void APA_Player_Skill_ArrowRain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APA_Player_Skill_ArrowRain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void APA_Player_Skill_ArrowRain::SetArrowRainProperty(int32 NewLevel)
{
	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();

	//프로퍼티 설정
	CurrentPropertyData = DefaultSetting->GetArrowRainSkillData(NewLevel);
	if (CurrentPropertyData != nullptr)
	{
		CoolTime = CurrentPropertyData->CoolTime;
		Mana = CurrentPropertyData->Mana;
		Damage = CurrentPropertyData->Damage;
		Coefficient = CurrentPropertyData->Coefficient;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ArrowRain`s PropertyData is nullptr"));
	}
}

void APA_Player_Skill_ArrowRain::SetArrowRainDamage(float PlayerAttackDamage)
{
	CoefficientDamage = PlayerAttackDamage * Coefficient;
}

void APA_Player_Skill_ArrowRain::DoDamage(AActor* TakeDamagedActor)
{
	auto PlayerControllerObject = Cast<APA_Player_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TSubclassOf<UDamageType> DamageTypeClass;
	UGameplayStatics::ApplyDamage(TakeDamagedActor, CoefficientDamage, PlayerControllerObject, this, DamageTypeClass);
}