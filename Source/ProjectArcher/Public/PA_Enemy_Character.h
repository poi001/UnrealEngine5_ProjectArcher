// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PA_Enemy_Character.generated.h"

//ĳ���� ���� Enum
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,	//ĳ���� ���� ���� ������Ʈ. �ּ��� ������ ������ ĳ���Ϳ� UI �� ���ܵ�
	LOADING,	//ĳ���� �ּ��� �ε�. ���� ��Ʈ�ѷ��� AI ���� �÷��̾����� ���� ����. �÷��̾� ��Ʈ�ѷ��� ��� �ּ� �ε��� �Ϸ�� ������ ĳ���͸� �������� ���ϵ��� �Է��� ��Ȱ��ȭ.
	READY,		//ĳ���� �ּ� �ε��� �Ϸ�� ������Ʈ. ���ܵ� ĳ���Ϳ� UI �� �����ְ�, ������� �Դ´�. �÷��̾� ��Ʈ�ѷ��� ���� ĳ���͸� �����ϰ�, AI ��Ʈ�ѷ��� �����̺�� Ʈ�� ������ ���� ĳ���͸� ���۽�Ų��.
	DEAD		//HP �� ������ ����ϸ� �״� �ִϸ��̼��� ��� / UI ���� / �浹 ��� ���ֱ� / ����� ���� �ʵ��� ����. ��Ʈ�ѷ��� �÷��̾��� ��� �Է��� ��Ȱ���ϰ� AI �� ��� �����̺�� Ʈ�� ������ �����Ѵ�. ���� �ð��� ������ �÷��̾�� ������ϰ� AI �� �������� �����Ѵ�.
};

UCLASS()
class PROJECTARCHER_API APA_Enemy_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APA_Enemy_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�÷��̾ ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Detect", Meta = (AllowPrivateAccess = true))
	bool bIsDetected = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//������� ���� ��, ó���ϴ� ���� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	//������Ʈ
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	class UPA_Enemy_EnemyStat* EnemyStat;				//�� ���� ���� ������Ʈ

	//AI
	UPROPERTY()
	class APA_Enemy_Controller* EnemyAI;

	//�ִϸ��̼�
	UPROPERTY()
	class UPA_Enemy_AnimInstance* EnemyAnimInstance;

	//�ڽ��� ���� �÷��̾ ���� ����
	UPROPERTY()
	AActor* DamagedPlayerCharacter;

	//ĳ���� ���� ����
	void SetEnemyState(ECharacterState NewState);

	//HPWidget ���� ���� ( �ܺο��� ���뵵 )
	void SetHPWidgetHidden(bool bHPWidgetHidden);

protected:
	//ĳ���� ����
	ECharacterState CharacterState;

	//����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", Meta = (AllowProtectedAccess = true))
	class UWidgetComponent* EnemyHPWidget;					//HP����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", Meta = (AllowProtectedAccess = true))
	class UWidgetComponent* DamageWidget;					//����� ����

	//����� ���Ŀ� ó���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "State", Meta = (AllowProtectedAccess = true))
	float DeadTimer;
	FTimerHandle DeadTimerHandle = { };

	//�÷��̾� ��Ʈ�ѷ�
	UPROPERTY()
	class APA_Player_PlayerController* PlayerController;

	//�����Լ� ( ����� �Լ� )
	virtual void AttackCheck() {};
	virtual void AttackEnd() {};
};
