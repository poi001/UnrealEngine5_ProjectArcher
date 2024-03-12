// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PA_Enemy_Character.generated.h"

//캐릭터 상태 Enum
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,	//캐릭터 생성 전의 스테이트. 애셋은 설정돼 있으나 캐릭터와 UI 를 숨겨둠
	LOADING,	//캐릭터 애셋을 로딩. 현재 컨트롤러가 AI 인지 플레이어인지 구분 가능. 플레이어 컨트롤러인 경우 애셋 로딩이 완료될 때까지 캐릭터를 조종하지 못하도록 입력을 비활성화.
	READY,		//캐릭터 애셋 로딩이 완료된 스테이트. 숨겨둔 캐릭터와 UI 를 보여주고, 대미지도 입는다. 플레이어 컨트롤러를 통해 캐릭터를 조종하고, AI 컨트롤러는 비헤이비어 트리 로직을 통해 캐릭터를 동작시킨다.
	DEAD		//HP 를 소진해 사망하면 죽는 애니메이션을 재생 / UI 끄기 / 충돌 기능 없애기 / 대미지 입지 않도록 설정. 컨트롤러가 플레이어인 경우 입력을 비활성하고 AI 인 경우 비헤이비어 트리 로직을 중지한다. 일정 시간이 지나면 플레이어는 재시작하고 AI 는 레벨에서 퇴장한다.
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

	//플레이어를 감지
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Detect", Meta = (AllowPrivateAccess = true))
	bool bIsDetected = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//대미지를 받을 때, 처리하는 가상 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	//컴포넌트
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	class UPA_Enemy_EnemyStat* EnemyStat;				//적 스탯 액터 컴포넌트

	//AI
	UPROPERTY()
	class APA_Enemy_Controller* EnemyAI;

	//애니메이션
	UPROPERTY()
	class UPA_Enemy_AnimInstance* EnemyAnimInstance;

	//자신을 때린 플레이어를 담을 변수
	UPROPERTY()
	AActor* DamagedPlayerCharacter;

	//캐릭터 상태 변경
	void SetEnemyState(ECharacterState NewState);

	//HPWidget 숨김 설정 ( 외부에서 사용용도 )
	void SetHPWidgetHidden(bool bHPWidgetHidden);

protected:
	//캐릭터 상태
	ECharacterState CharacterState;

	//위젯
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", Meta = (AllowProtectedAccess = true))
	class UWidgetComponent* EnemyHPWidget;					//HP위젯
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", Meta = (AllowProtectedAccess = true))
	class UWidgetComponent* DamageWidget;					//대미지 위젯

	//사망한 이후에 처리할 로직
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "State", Meta = (AllowProtectedAccess = true))
	float DeadTimer;
	FTimerHandle DeadTimerHandle = { };

	//플레이어 컨트롤러
	UPROPERTY()
	class APA_Player_PlayerController* PlayerController;

	//가상함수 ( 상속할 함수 )
	virtual void AttackCheck() {};
	virtual void AttackEnd() {};
};
