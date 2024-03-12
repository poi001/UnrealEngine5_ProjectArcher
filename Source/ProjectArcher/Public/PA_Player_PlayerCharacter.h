// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PA_Player_PlayerCharacter.generated.h"

//플레이어 스킬 상태 Enum
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
	// 캐릭터 초기화 이후
	virtual void PostInitializeComponents() override;
	//대미지를 받을 때, 처리하는 가상 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	//스탯 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	class UPA_Player_PlayerStat* CharacterStat;

private:

	////////////////////////////////////////////////카메라////////////////////////////////////////////////
	
	//스프링암 ( 카메라붐 )
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* SpringArm;
	//카메라
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;
	//조준 카메라
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = true))
	class UCameraComponent* FocusCamera;

	////////////////////////////////////////////////평타 조준 관련////////////////////////////////////////////////

	//HUD중앙 부분 위치( 카메라 위치 )
	FVector CrosshairWorldLocation;
	//트레이스해서 맞는 부분
	FVector ImpactPoint;
	//화살이 나가는 위치
	FVector ArrowSpawnLocation;
	//화살이 나갈때 회전율
	FRotator ArrowSpawnRotator;
	//라인 트레이싱의 히트 결과
	bool bIsHitByTrace;
	//발사 행동 확인용 변수
	bool CanFire = true;
	//오른쪽 클릭을 이용한 조준 스킬을 사용하고 있는지 확인용 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Focus", Meta = (AllowPrivateAccess = true))
	bool IsFocusing;
	//모으기
	bool IsCharging = false;
	//모으기 중인 시간
	float ChargingTime = 0.0f;
	//법선 벡터
	FVector NormalVector = FVector::ZeroVector;

	//목표점 조정 함수
	void FindImpactPoint();

	////////////////////////////////////////////////평타, 스킬 오브젝트 및 관련 변수////////////////////////////////////////////////
	
	//평타 화살 블루프린트 클래스
	UPROPERTY()
	TSubclassOf<class APA_Object_PlayerArrow> Arrow;					//화살
	UPROPERTY()
	TSubclassOf<class APA_Object_PlayerArrow> AdvancedArrow;			//강화 화살

	//Q스킬
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Boom> Boom;			//폭탄
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Boom", Meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Boom_Sphere;
	bool OnBoomTrajectory = false;							//폭탄 궤적을 띄울지 확인용 변수

	//E스킬
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Block> Block;		//블록

	//R스킬
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_ArrowRain> ArrowRain;//화살비
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ArrowRain", Meta = (AllowPrivateAccess = true))
	class UDecalComponent* ArrowRain_Decal;					//화살비 데칼
	bool OnArrowRainDecal = false;							//화살비 범위를 띄울지 확인용 변수

	//G스킬
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Teleport> Teleport;	//텔레포트
	UPROPERTY()
	class APA_Player_Skill_Teleport* TeleportObject;		//텔레포트 오브젝트

	//Shift스킬
	UPROPERTY()
	TSubclassOf<class APA_Player_Skill_Dash> Dash;			//대시
	FTimerHandle DashHandle;								//타이머 핸들

	//캐릭터가 현재 사용하고 있는 스킬 상태
	ESkillState SkillState = ESkillState::Idle;

	////////////////////////////////////////////////애님 인스턴스////////////////////////////////////////////////

	//애님 인스턴스
	UPROPERTY()
	class UPA_Player_PlayerAnimInstance* PlayerAnim;	//플레이어 애님 인스턴스

	////////////////////////////////////////////////위젯////////////////////////////////////////////////

	//일반 상태 조준점
	UPROPERTY()
	TSubclassOf<class UUserWidget> CrosshairsClass;				//Class
	UPROPERTY()
	class UUserWidget* CrosshairsWidget;						//Object

	//조준 상태 조준점
	UPROPERTY()
	TSubclassOf<class UUserWidget> FocusingCrosshairsClass;		//Class
	UPROPERTY()
	class UUserWidget* FocusingCrosshairsWidget;				//Object

	//플레이어 UI
	UPROPERTY()
	TSubclassOf<class UPA_Player_UI> PlayerUIWidgetClass;		//Class
	UPROPERTY()
	class UPA_Player_UI* PlayerUIWidget;						//Object

	//조준점 띄우는 함수
	UFUNCTION()
	void SetCrosshairs(bool bIsFocusing);


	////////////////////////////////////////////////액션////////////////////////////////////////////////
	
	//점프
	void StartJump();					//점프 시작
	void StopJump();					//점프 정지
	//일반 공격
	void Focus();						//화살 조준 ( 일반 공격 )
	void Fire();						//화살 발사 ( 일반 공격 )
	//오른쪽 마우스 클릭 스킬
	void Skill_RightMouseClick();		//오른쪽 마우스 클릭 스킬 조준 ( 줌 )
	//Q스킬
	void Skill_Q_Focus();				//Q스킬 조준 ( 폭탄 )
	void Skill_Q_Fire();				//Q스킬 발사 ( 폭탄 )
	void OnTrajectory_Boom();			//폭탄 궤적 On
	//E스킬
	void Skill_E();						//E스킬 ( 블록 )
	//R스킬
	void Skill_R();						//R스킬 ( 화살비 )
	void OnDecal_ArrowRain();			//화살비 데칼 On
	//Shift스킬
	void Skill_Shift();					//Shift스킬 ( 대시 )
	//G스킬
	void Skill_G();						//G스킬 ( 텔레포트 )

	//플레이어가 죽을 때 관련
	void PlayerDead();					//사망할 시, 호출되는 함수
	float DeadTimer = 1.5f;				//사망하고 UI가 뜰 시간
	FTimerHandle DeadTimerHandle = { };	//타이머 핸들

	////////////////////////////////////////////////축 매핑////////////////////////////////////////////////
	
	//앞, 뒤 이동
	void MoveForward(float NewAxisValue);
	//우, 좌 이동
	void MoveRight(float NewAxisValue);
	//시야 ( 위, 아래 )
	void LookUp(float NewAxisValue);
	//시야 ( 좌, 우 )
	void Turn(float NewAxisValue);

	////////////////////////////////////////////////레벨 업////////////////////////////////////////////////
	void LevelUp_fun();						//레벨업 시에 호출될 함수

protected:
	//레벨업
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Widget", Meta = (AllowProtectedAccess = true))
	class UWidgetComponent* LevelUpWidgetComponent;						//레벨업 위젯
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Particle", Meta = (AllowProtectedAccess = true))
	class UNiagaraComponent* LevelUpNiagara;							//레벨업 파티클( 나이아가라 )
};
