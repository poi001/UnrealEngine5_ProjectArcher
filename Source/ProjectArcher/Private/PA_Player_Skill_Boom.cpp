// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_Skill_Boom.h"
#include "PA_Setting.h"

// Sets default values
APA_Player_Skill_Boom::APA_Player_Skill_Boom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APA_Player_Skill_Boom::BeginPlay()
{
	Super::BeginPlay();
	
	//������Ƽ �ʱ�ȭ
	SetBoomProperty(1);
}

// Called every frame
void APA_Player_Skill_Boom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APA_Player_Skill_Boom::SetBoomProperty(int32 NewLevel)
{
	//���ð��� ������Ʈ�� �����Ѵ�
	auto DefaultSetting = GetDefault<UPA_Setting>();

	//������Ƽ ����
	CurrentPropertyData = DefaultSetting->GetBoomSkillData(NewLevel);
	if (CurrentPropertyData != nullptr)
	{
		CoolTime = CurrentPropertyData->CoolTime;
		Mana = CurrentPropertyData->Mana;
		Damage = CurrentPropertyData->Damage;
		Coefficient = CurrentPropertyData->Coefficient;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Boom`s PropertyData is nullptr"));
	}
}