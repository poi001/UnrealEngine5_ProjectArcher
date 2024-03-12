// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_Player_Skill_Dash.generated.h"

UCLASS()
class PROJECTARCHER_API APA_Player_Skill_Dash : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APA_Player_Skill_Dash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//������Ƽ
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float CoolTime;			//���� ���ð�
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float Mana;				//���� ����
	UPROPERTY(Transient, BlueprintReadOnly, VisibleDefaultsOnly, Category = "Boom")
	float Speed;			//���ǵ�

	//Set �Լ�
	void SetDashProperty(int32 NewLevel);							//������ �̿��� ������Ƽ ����

private:
	struct FDashSkillDataTable* CurrentPropertyData = nullptr;
};
