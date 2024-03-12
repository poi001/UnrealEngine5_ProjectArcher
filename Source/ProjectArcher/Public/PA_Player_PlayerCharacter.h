// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PA_Player_PlayerCharacter.generated.h"

//�÷��̾� ��ų ���� Enum
UENUM(BlueprintType)
enum class ESkillState : uint8
{
	Idle,
	LeftMouse,
	RightMouse,
	Q,
	E,
	R,
	G,
	Shift
};

UCLASS()
class PROJECTARCHER_API APA_Player_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APA_Player_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ĳ���� �ʱ�ȭ ����
	virtual void PostInitializeComponents() override;
	//������� ���� ��, ó���ϴ� ���� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	//���� ������Ʈ
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	class UPA_Player_PlayerStat* CharacterStat;

private:

	////////////////////////////////////////////////ī�޶�////////////////////////////////////////////////
	
	//�������� ( ī�޶�� )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm;
	//ī�޶�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;
	//���� ī�޶�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class UCameraComponent* FocusCamera;

	////////////////////////////////////////////////��Ÿ ���� ����////////////////////////////////////////////////

	//HUD�߾� �κ� ��ġ( ī�޶� ��ġ )
	FVector CrosshairWorldLocation;
	//Ʈ���̽��ؼ� �´� �κ�
	FVector ImpactPoint;
	//ȭ���� ������ ��ġ
	FVector ArrowSpawnLocation;
	//ȭ���� ������ ȸ����
	FRotator ArrowSpawnRotator;
	//���� Ʈ���̽��� ��Ʈ ���
	bool bIsHitByTrace;
	//�߻� �ൿ Ȯ�ο� ����
	bool CanFire = true;
	//������ Ŭ���� �̿��� ���� ��ų�� ����ϰ� �ִ��� Ȯ�ο� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Focus", Meta = (AllowPrivateAccess = true))
	bool IsFocusing;
	//������
	bool IsCharging = false;
	//������ ���� �ð�
	float ChargingTime = 0.0f;
	//���� ����
	FVector NormalVector = FVector::ZeroVector;

	//��ǥ�� ���� �Լ�
	void FindImpactPoint();

	////////////////////////////////////////////////��Ÿ, ��ų ������Ʈ �� ���� ����////////////////////////////////////////////////
	
	//��Ÿ ȭ�� �������Ʈ Ŭ����
	UPROPERTY()
	TSubclassOf<class APA_Object_PlayerArrow> Arrow;					//ȭ��
	UPROPERTY()
	TSubclassOf<class APA_Object_PlayerArrow> AdvancedArrow;			//��ȭ ȭ��

	//Q��ų
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Boom> Boom;			//��ź
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Boom", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Boom_Sphere;
	bool OnBoomTrajectory = false;							//��ź ������ ����� Ȯ�ο� ����

	//E��ų
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Block> Block;		//���

	//R��ų
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_ArrowRain> ArrowRain;//ȭ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ArrowRain", Meta = (AllowPrivateAccess = true))
	class UDecalComponent* ArrowRain_Decal;					//ȭ��� ��Į
	bool OnArrowRainDecal = false;							//ȭ��� ������ ����� Ȯ�ο� ����

	//G��ų
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Teleport> Teleport;	//�ڷ���Ʈ
	UPROPERTY()
	class APA_Player_Skill_Teleport* TeleportObject;		//�ڷ���Ʈ ������Ʈ

	//Shift��ų
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Dash> Dash;			//���
	FTimerHandle DashHandle;								//Ÿ�̸� �ڵ�

	//ĳ���Ͱ� ���� ����ϰ� �ִ� ��ų ����
	ESkillState SkillState = ESkillState::Idle;

	////////////////////////////////////////////////�ִ� �ν��Ͻ�////////////////////////////////////////////////

	//�ִ� �ν��Ͻ�
	UPROPERTY()
	class UPA_Player_PlayerAnimInstance* PlayerAnim;	//�÷��̾� �ִ� �ν��Ͻ�

	////////////////////////////////////////////////����////////////////////////////////////////////////

	//�Ϲ� ���� ������
	UPROPERTY()
	TSubclassOf<class UUserWidget> CrosshairsClass;				//Class
	UPROPERTY()
	class UUserWidget* CrosshairsWidget;						//Object

	//���� ���� ������
	UPROPERTY()
	TSubclassOf<class UUserWidget> FocusingCrosshairsClass;		//Class
	UPROPERTY()
	class UUserWidget* FocusingCrosshairsWidget;				//Object

	//�÷��̾� UI
	UPROPERTY()
	TSubclassOf<class UPA_Player_UI> PlayerUIWidgetClass;		//Class
	UPROPERTY()
	class UPA_Player_UI* PlayerUIWidget;						//Object

	//������ ���� �Լ�
	UFUNCTION()
	void SetCrosshairs(bool bIsFocusing);


	////////////////////////////////////////////////�׼�////////////////////////////////////////////////
	
	//����
	void StartJump();					//���� ����
	void StopJump();					//���� ����
	//�Ϲ� ����
	void Focus();						//ȭ�� ���� ( �Ϲ� ���� )
	void Fire();						//ȭ�� �߻� ( �Ϲ� ���� )
	//������ ���콺 Ŭ�� ��ų
	void Skill_RightMouseClick();		//������ ���콺 Ŭ�� ��ų ���� ( �� )
	//Q��ų
	void Skill_Q_Focus();				//Q��ų ���� ( ��ź )
	void Skill_Q_Fire();				//Q��ų �߻� ( ��ź )
	void OnTrajectory_Boom();			//��ź ���� On
	//E��ų
	void Skill_E();						//E��ų ( ��� )
	//R��ų
	void Skill_R();						//R��ų ( ȭ��� )
	void OnDecal_ArrowRain();			//ȭ��� ��Į On
	//Shift��ų
	void Skill_Shift();					//Shift��ų ( ��� )
	//G��ų
	void Skill_G();						//G��ų ( �ڷ���Ʈ )

	//�÷��̾ ���� �� ����
	void PlayerDead();					//����� ��, ȣ��Ǵ� �Լ�
	float DeadTimer = 1.5f;				//����ϰ� UI�� �� �ð�
	FTimerHandle DeadTimerHandle = { };	//Ÿ�̸� �ڵ�

	////////////////////////////////////////////////�� ����////////////////////////////////////////////////
	
	//��, �� �̵�
	void MoveForward(float NewAxisValue);
	//��, �� �̵�
	void MoveRight(float NewAxisValue);
	//�þ� ( ��, �Ʒ� )
	void LookUp(float NewAxisValue);
	//�þ� ( ��, �� )
	void Turn(float NewAxisValue);

	////////////////////////////////////////////////���� ��////////////////////////////////////////////////
	void LevelUp_fun();						//������ �ÿ� ȣ��� �Լ�

protected:
	//������
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widget", Meta = (AllowProtectedAccess = true))
	class UWidgetComponent* LevelUpWidgetComponent;						//������ ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Particle", Meta = (AllowProtectedAccess = true))
	class UNiagaraComponent* LevelUpNiagara;							//������ ��ƼŬ( ���̾ư��� )
};
