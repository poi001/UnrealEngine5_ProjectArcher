// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Character_Zombie.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_AnimInstance.h"
#include "PA_Enemy_EnemyStat.h"

#include "Components/WidgetComponent.h"					//위젯 컴포넌트
#include "DrawDebugHelpers.h"							//드로우디버그
#include "Engine/DamageEvents.h"						//DamageEvent
#include "GameFramework/CharacterMovementComponent.h"	//캐릭터무브먼트


APA_Enemy_Character_Zombie::APA_Enemy_Character_Zombie()
{
	//기본 애님 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UPA_Enemy_AnimInstance> ENEMY_ANIM(TEXT("/Game/Character/Enemy/AnimBP/AnimBP_Zombie.AnimBP_Zombie_C"));
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	//위젯
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));			//위치는 캐릭터 머리위로
	DamageWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));				//위치는 HPBar 위로
}

void APA_Enemy_Character_Zombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnimInstance = Cast<UPA_Enemy_AnimInstance>(GetMesh()->GetAnimInstance());
	//공격 판정 델리게이트 설정
	EnemyAnimInstance->OnAttackCheckDelegate.AddUObject(this, &APA_Enemy_Character_Zombie::AttackCheck);
	//공격 끝날 시 델리게이트 설정
	EnemyAnimInstance->OnAttackEndDelegate.AddUObject(this, &APA_Enemy_Character_Zombie::AttackEnd);
}

void APA_Enemy_Character_Zombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AI 변수에 현재 AI를 넣는다
	EnemyAI = Cast<APA_Enemy_Controller>(NewController);
}
//start
void APA_Enemy_Character_Zombie::BeginPlay()
{
	Super::BeginPlay();

	//적 캐릭터 상태 변경 //비동기 에셋으로 불러오고 완료될 때까지 LOADING 상태로 있다가 READY로 바꾼다
	EnemyStat->SetNewCode(1);
	GetCharacterMovement()->MaxWalkSpeed = EnemyStat->GetSpeed();		//이동속도 설정
	EnemyAI->SetZombie();
	SetEnemyState(ECharacterState::READY);
}

void APA_Enemy_Character_Zombie::AttackCheck()
{
	FHitResult HitResult;
	//FCollisionQueryParams 인자
	//1. 태그 네이밍, 2. 추적 복잡성 여부(디테일한 판정이 아니라면 false 추천), 3. 갖고 있는 무시할 액터
	FCollisionQueryParams Params(NAME_None, false, this);
	float AttackRange = EnemyStat->GetAttackRange();

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,														//물리적 충돌이 탐지된 경우 관련된 정보를 담을 구조체
		GetActorLocation(),												//탐색을 시작할 위치
		GetActorLocation() + GetActorForwardVector() * AttackRange,		//탐색을 끝낼 위치
		FQuat::Identity,												//탐색에 사용할 도형의 회전
		ECollisionChannel::ECC_GameTraceChannel4,						//물리 충돌 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(AttackRadius),						//탐색에 사용할 도형 정보
		Params															//탐색 방법에 대한 설정 값을 모아둔 구조체
	);
	/*
#if ENABLE_DRAW_DEBUG	//디버그 드로잉을 구현할 구절
	FVector TraceVec = GetActorForwardVector() * AttackRange;			//트레이스 벡터
	FVector Center = GetActorLocation() + TraceVec * 0.5f;				//캡슐의 가운데 점
	float HalfHeight = AttackRange * 0.5f + AttackRadius;				//길이의 절반
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();	//캡슐의 회전
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;			//캡슐의 색깔
	float DebugLifeTime = 3.0f;											//캡슐이 나타날 지속 시간

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif
*/
	if (bResult)
	{
		if (HitResult.GetActor())
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(EnemyStat->GetAttackDamage(), DamageEvent, GetController(), this);//상대에게 피해를 준다
		}
	}
}

void APA_Enemy_Character_Zombie::AttackEnd()
{
	EnemyAnimInstance->IsAttack = false;
}