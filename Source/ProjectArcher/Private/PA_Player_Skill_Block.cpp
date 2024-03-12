// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_Skill_Block.h"
#include "PA_Setting.h"

// Sets default values
APA_Player_Skill_Block::APA_Player_Skill_Block()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APA_Player_Skill_Block::BeginPlay()
{
	Super::BeginPlay();
	
	//������Ƽ �ʱ�ȭ
	SetBlockProperty(1);
}

// Called every frame
void APA_Player_Skill_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APA_Player_Skill_Block::SetBlockProperty(int32 NewLevel)
{
	//���ð��� ������Ʈ�� �����Ѵ�
	auto DefaultSetting = GetDefault<UPA_Setting>();

	//������Ƽ ����
	CurrentPropertyData = DefaultSetting->GetBlockSkillData(NewLevel);
	if (CurrentPropertyData != nullptr)
	{
		CoolTime = CurrentPropertyData->CoolTime;
		Mana = CurrentPropertyData->Mana;
		Extent = CurrentPropertyData->Extent;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Block`s PropertyData is nullptr"));
	}
}

void APA_Player_Skill_Block::BlockRiseUp_Implementation()
{

}