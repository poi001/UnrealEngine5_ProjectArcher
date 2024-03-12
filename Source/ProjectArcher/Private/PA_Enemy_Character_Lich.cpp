// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_Character_Lich.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_EnemyStat.h"
#include "PA_Enemy_AnimInstance_Lich.h"

#include "Components/WidgetComponent.h"					//위젯 컴포넌트
#include "DrawDebugHelpers.h"							//드로우디버그
#include "Engine/DamageEvents.h"						//DamageEvent
#include "GameFramework/CharacterMovementComponent.h"	//캐릭터무브먼트
#include "Kismet/GameplayStatics.h"						//Kimset


APA_Enemy_Character_Lich::APA_Enemy_Character_Lich()
{
	//기본 애님 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UPA_Enemy_AnimInstance> ENEMY_ANIM(TEXT("/Game/Character/Enemy/AnimBP/AnimBP_Lich.AnimBP_Lich_C"));
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	//위젯
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));			//위치는 캐릭터 머리위로
	DamageWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 260.0f));				//위치는 HPBar 위로

	//프로퍼티
	LichDamage = 0.0f;
}

void APA_Enemy_Character_Lich::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnimInstance = Cast<UPA_Enemy_AnimInstance>(GetMesh()->GetAnimInstance());
	//공격 판정 델리게이트 설정
	EnemyAnimInstance->OnAttackCheckDelegate.AddUObject(this, &APA_Enemy_Character_Lich::AttackCheck);
	//공격 끝날 시 델리게이트 설정
	EnemyAnimInstance->OnAttackEndDelegate.AddUObject(this, &APA_Enemy_Character_Lich::AttackEnd);
	//Ray스킬 델리게이트

}

void APA_Enemy_Character_Lich::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AI 변수에 현재 AI를 넣는다
	EnemyAI = Cast<APA_Enemy_Controller>(NewController);
}
//start
void APA_Enemy_Character_Lich::BeginPlay()
{
	Super::BeginPlay();

	//적 캐릭터 상태 변경 //비동기 에셋으로 불러오고 완료될 때까지 LOADING 상태로 있다가 READY로 바꾼다
	EnemyStat->SetNewCode(2);
	GetCharacterMovement()->MaxWalkSpeed = EnemyStat->GetSpeed();		//이동속도 설정
	LichDamage = EnemyStat->GetAttackDamage();							//공격력 설정
	EnemyAI->SetLich();

	//Lich 캐릭터의 Ray 스킬 델리게이트
	auto LichAnimInstace = Cast<UPA_Enemy_AnimInstance_Lich>(EnemyAnimInstance);
	LichAnimInstace->OnRayStartDelegate.AddUObject(this, &APA_Enemy_Character_Lich::OnRaySkill);
	LichAnimInstace->OnRayEndDelegate.AddUObject(this, &APA_Enemy_Character_Lich::OffRaySkill);

	//Lich 캐릭터의 텔레포트 델리게이트
	LichAnimInstace->OnTeleport.AddUObject(this, &APA_Enemy_Character_Lich::LichTeleportToPlayer);
	
	//애님 인스턴스 널체크
	if (EnemyAnimInstance == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("EnemyAnimInstance is nullptr"));
	}

	//타이머 핸들
	FTimerHandle EmergenceTimerHandle;
	//Lich가 등장할 때
	GetWorld()->GetTimerManager().SetTimer(
		EmergenceTimerHandle,
		FTimerDelegate::CreateLambda(
			[this, LichAnimInstace]()->void
			{
				LichAnimInstace->IsEmerging = false;
				SetEnemyState(ECharacterState::READY);
			}),
		2.25f, false);
}

void APA_Enemy_Character_Lich::AttackCheck()
{
	FHitResult HitResult;

	//FCollisionQueryParams 인자
	//1. 태그 네이밍, 2. 추적 복잡성 여부(디테일한 판정이 아니라면 false 추천), 3. 갖고 있는 무시할 액터
	FCollisionQueryParams Params(NAME_None, false, this);

	//공격 사거리
	float AttackRange = EnemyStat->GetAttackRange();

	//트레이스 벡터
	FVector TraceVec = GetActorForwardVector() * AttackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,														//물리적 충돌이 탐지된 경우 관련된 정보를 담을 구조체
		GetActorLocation(),												//탐색을 시작할 위치
		GetActorLocation() + TraceVec,									//탐색을 끝낼 위치
		FRotationMatrix::MakeFromY(TraceVec).ToQuat(),					//탐색에 사용할 도형의 회전
		ECollisionChannel::ECC_GameTraceChannel4,						//물리 충돌 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(AttackRadius),						//탐색에 사용할 도형 정보
		Params															//탐색 방법에 대한 설정 값을 모아둔 구조체
	);
	/*
#if ENABLE_DRAW_DEBUG	//디버그 드로잉을 구현할 구절
	FVector Center = GetActorLocation() + TraceVec * 0.5f;				//캡슐의 가운데 점
	float HalfHeight = AttackRange * 0.5f + AttackRadius;				//길이의 절반
	FQuat CapsuleRot = FRotationMatrix::MakeFromY(TraceVec).ToQuat();	//캡슐의 회전
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

void APA_Enemy_Character_Lich::AttackEnd()
{
	EnemyAnimInstance->IsAttack = false;
}

void APA_Enemy_Character_Lich::LichTeleportToPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Teleport"));
	SetActorLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
}