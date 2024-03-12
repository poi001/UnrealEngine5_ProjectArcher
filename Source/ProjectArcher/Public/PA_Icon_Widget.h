// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PA_Setting.h"
#include "Blueprint/UserWidget.h"
#include "PA_Icon_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARCHER_API UPA_Icon_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//��ų ������
	UPROPERTY(Meta = (BindWidget))
	class UImage* Image_Icon;

	//��Ÿ�� �ؽ�Ʈ
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* TextBlock_CoolTime;

	//��Ÿ�� ���α׷�����
	UPROPERTY(Meta = (BindWidget))
	class UProgressBar* ProgressBar_CoolTime;

	//���� ���ð�
	UPROPERTY()
	float CoolTime = 0.0f;

	//���� �ð�
	UPROPERTY()
	float RemainingTime;

	//�� ������ ��ų
	UPROPERTY()
	ESkill Skill;

public:
	//�� �������� ��ų�� ��ȯ��
	ESkill GetSkill();
	//���� ���ð� ����
	void SetSkill(ESkill ESkill, float fCoolTime);		//��ų�� �ʱ�ȭ��

	//��ų�� ������� ��
	void ActiveSkill();

	//��Ÿ������ Ȯ�ο� ����
	bool IsActive = false;

private:
	FSoftObjectPath SkillIconToLoad = FSoftObjectPath(nullptr);				//��ų ������ ���

	//������ ���α׷����� ����
	void SetProgressBar();
};
