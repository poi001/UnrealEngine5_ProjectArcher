// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_Skill_Dash.h"
#include "PA_Setting.h"

// Sets default values
APA_Player_Skill_Dash::APA_Player_Skill_Dash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APA_Player_Skill_Dash::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APA_Player_Skill_Dash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APA_Player_Skill_Dash::SetDashProperty(int32 NewLevel)
{
	//세팅관련 오브젝트를 선언한다
	auto DefaultSetting = GetDefault<UPA_Setting>();

	//프로퍼티 설정
	CurrentPropertyData = DefaultSetting->GetDashSkillData(NewLevel);
	if (CurrentPropertyData != nullptr)
	{
		CoolTime = CurrentPropertyData->CoolTime;
		Mana = CurrentPropertyData->Mana;
		Speed = CurrentPropertyData->Speed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Dash`s PropertyData is nullptr"));
	}
}