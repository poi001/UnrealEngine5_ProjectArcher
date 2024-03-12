// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Player_PlayerCharacter.h"
#include "PA_Player_PlayerAnimInstance.h"
#include "PA_Object_PlayerArrow.h"
#include "PA_Player_PlayerStat.h"
#include "PA_Player_PlayerController.h"
#include "PA_Player_UI.h"
#include "PA_Player_PlayerStat.h"
#include "PA_Player_Skill_Teleport.h"
#include "PA_Player_Skill_Boom.h"
#include "PA_Player_Skill_Block.h"
#include "PA_Player_Skill_ArrowRain.h"
#include "PA_Player_Skill_Dash.h"
#include "PA_Player_LevelUpWidget.h"

#include "Kismet/GameplayStatics.h"						//UGameplayStatics
#include "GameFramework/SpringArmComponent.h"			//스프링암
#include "Camera/CameraComponent.h"						//카메라
#include "Components/CapsuleComponent.h"				//캡슐컴포넌트
#include "GameFramework/CharacterMovementComponent.h"	//캐릭터무브먼트
#include "Blueprint/UserWidget.h"						//위젯
#include "Kismet/KismetMathLibrary.h"					//MakeRotFromX
#include "Components/DecalComponent.h"					//데칼 컴포넌트
#include "Components/WidgetComponent.h"					//위젯 컴포넌트
#include "Particles/ParticleSystem.h"					//파티클 시스템
#include "NiagaraFunctionLibrary.h"						//나이아가라 함수
#include "NiagaraComponent.h"							//나이아가라 컴포넌트


// Sets default values
APA_Player_PlayerCharacter::APA_Player_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));				//CameraBoom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));						//Camera
	FocusCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOCUSCAMERA"));			//FocusCamera
	CharacterStat = CreateDefaultSubobject<UPA_Player_PlayerStat>(TEXT("CHARACTERSTAT"));	//스탯
	Boom_Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOOM_SPHERE"));		//폭탄 반경 메시
	ArrowRain_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("ARROWRAIN_DECAL"));		//화살비 반경 데칼
	LevelUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LEVELUP"));		//레벨업 시 재생될 위젯 컴포넌트
	LevelUpNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LEVELUPNIAGARA"));		//레벨업 나이아가라

	//컴포넌트 부착
	SpringArm->SetupAttachment(GetCapsuleComponent());				//CameraBoom
	Camera->SetupAttachment(SpringArm);								//Camera
	FocusCamera->SetupAttachment(GetMesh());						//FocusCamera
	LevelUpWidgetComponent->SetupAttachment(GetMesh());				//레벨업 위젯
	LevelUpNiagara->SetupAttachment(GetCapsuleComponent());

	//컴포넌트 설정
	Boom_Sphere->SetVisibility(false);
	ArrowRain_Decal->SetVisibility(false);
	
	//플레이어 컨트롤러 회전 설정 ( 컨트롤러에 맞춰서 캐릭터가 회전하는 설정 )
	bUseControllerRotationPitch = false;										// 컨트롤러의 Pitch값 사용 여부
	bUseControllerRotationYaw = true;											// 컨트롤러의 Yaw값 사용 여부
	bUseControllerRotationRoll = false;											// 컨트롤러의 Roll값 사용 여부

	//카메라 설정 ( SpringArm )
	SpringArm->TargetArmLength = 750;											// 스프링 암의 길이
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));				// 스프링 암의 회전율을 조정한다
	SpringArm->SocketOffset = FVector(0.0f, 120.0f, 75.0f);						// 스프링 암의 끝 부분을 조정한다
	SpringArm->bDoCollisionTest = true;											// 카메라가 벽에 충돌하는지에 대한 여부
	SpringArm->bUsePawnControlRotation = true;									// 스프링암이 폰의 로테이션 값을 사용할 것인지 여부
	SpringArm->bInheritPitch = true;											// 스프링 암이 회전율을 상속받는다. ( 카메라 )
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	FocusCamera->bUsePawnControlRotation = true;								// 카메라가 폰의 로테이션 값을 사용할 것인지 여부
	Camera->SetActive(true);													//Camera를 기본 카메라로 설정
	FocusCamera->SetActive(false);												//FocusCamera를 기본 카메라로 설정하지 않음

	//캐릭터 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;					// 가고 있는 방향 쪽으로 캐릭터가 쳐다 봄
	GetCharacterMovement()->JumpZVelocity = 600.0f;								// 점프 높이
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;								// 이동 속도

	//기본 스켈레탈 메쉬 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MANNEQUIN(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (MANNEQUIN.Succeeded()) GetMesh()->SetSkeletalMesh(MANNEQUIN.Object);
	//스켈레탈 메쉬 위치 및 회전 조절
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//애니메이션 모드를 애니메이션 블루프린트로 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//기본 애님 인스턴스 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Character/Player/AnimBP/AnimBP_PlayerCharacter.AnimBP_PlayerCharacter_C"));
	if (PLAYER_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);

	//화살
	static ConstructorHelpers::FClassFinder<APA_Object_PlayerArrow> ARROW(TEXT("/Game/Object/Arrow/BP/BP_Arrow.BP_Arrow_C"));
	if (ARROW.Succeeded()) Arrow = ARROW.Class;								//화살
	static ConstructorHelpers::FClassFinder<APA_Object_PlayerArrow> ADVANCEDARROW(TEXT("/Game/Object/Arrow/BP/BP_AdvancedArrow.BP_AdvancedArrow_C"));
	if (ADVANCEDARROW.Succeeded()) AdvancedArrow = ADVANCEDARROW.Class;		//강화 화살

	//위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> CROSSHAIR(TEXT("/Game/Widget/HUD/WBP_Crosshairs.WBP_Crosshairs_C"));
	if (CROSSHAIR.Succeeded()) CrosshairsClass = CROSSHAIR.Class;							//일반 조준점
	static ConstructorHelpers::FClassFinder<UUserWidget> FOCUSINGCROSSHAIR(TEXT("/Game/Widget/HUD/WBP_Crosshairs_Focus.WBP_Crosshairs_Focus_C"));
	if (FOCUSINGCROSSHAIR.Succeeded()) FocusingCrosshairsClass = FOCUSINGCROSSHAIR.Class;	//조준 조준점
	static ConstructorHelpers::FClassFinder<UPA_Player_UI> PLAYER_UI_C(TEXT("/Game/Widget/UI/WBP_UI.WBP_UI_C"));
	if (PLAYER_UI_C.Succeeded()) PlayerUIWidgetClass = PLAYER_UI_C.Class;					//플레이어 UI( HP, MP, EXP, Skill )

	LevelUpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);					//레벨업 위젯이 항상 스크린에 보이도록 설정
	static ConstructorHelpers::FClassFinder<UPA_Player_LevelUpWidget>LEVELUP_WIDGET(TEXT("/Game/Widget/LevelUp/WBP_LevelUp.WBP_LevelUp_C"));
	if (LEVELUP_WIDGET.Succeeded())
	{
		LevelUpWidgetComponent->SetWidgetClass(LEVELUP_WIDGET.Class);						//기본 클래스로 설정
		LevelUpWidgetComponent->SetDrawSize(FVector2D(256.0f, 75.0f));						//위젯의 크기 설정
	}

	//스킬
		//Boom
	static ConstructorHelpers::FClassFinder<APA_Player_Skill_Boom> BOOM(TEXT("/Game/Character/Player/Skill/BP_Skill_Boom.BP_Skill_Boom_C"));
	if (BOOM.Succeeded()) Boom = BOOM.Class;
		//Block
	static ConstructorHelpers::FClassFinder<APA_Player_Skill_Block> BLOCK(TEXT("/Game/Character/Player/Skill/BP_Skill_Block.BP_Skill_Block_C"));
	if (BLOCK.Succeeded()) Block = BLOCK.Class;
		//ArrowRain
	static ConstructorHelpers::FClassFinder<APA_Player_Skill_ArrowRain> ARROWRAIN(TEXT("/Game/Character/Player/Skill/BP_Skill_ArrowRain.BP_Skill_ArrowRain_C"));
	if (ARROWRAIN.Succeeded()) ArrowRain = ARROWRAIN.Class;
		//Teleport
	static ConstructorHelpers::FClassFinder<APA_Player_Skill_Teleport> TELEPORT(TEXT("/Game/Character/Player/Skill/BP_Skill_Teleport.BP_Skill_Teleport_C"));
	if (TELEPORT.Succeeded()) Teleport = TELEPORT.Class;
		//Shift
	static ConstructorHelpers::FClassFinder<APA_Player_Skill_Dash> DASH(TEXT("/Game/Character/Player/Skill/BP_Skill_Dash.BP_Skill_Dash_C"));
	if (DASH.Succeeded()) Dash = DASH.Class;
	
	//프로퍼티
	IsFocusing = false;
}

void APA_Player_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//애님 인스턴스를 PlayerAnim변수에 집어 넣는다
	PlayerAnim = Cast<UPA_Player_PlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnim == nullptr) UE_LOG(LogTemp, Error, TEXT("PlayerAnimInstance is nullptr"));	//null체크

	//발사 델리게이트
	PlayerAnim->OnResetCombo.AddLambda([this]()->void { 
		CanFire = true;
		SkillState = ESkillState::Idle;
		});

	//캐릭터 죽을 때, 델리게이트
	CharacterStat->OnHPIsZeroDelegate.AddUObject(this, &APA_Player_PlayerCharacter::PlayerDead);

	//레벨업을 할 때, 스킬들의 데이터 업데이트 델리게이트
	CharacterStat->OnLevelUpDelegate.AddUObject(this, &APA_Player_PlayerCharacter::LevelUp_fun);
}

// Called when the game starts or when spawned
void APA_Player_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//조준점 띄우기
	CrosshairsWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairsClass);
	FocusingCrosshairsWidget = CreateWidget<UUserWidget>(GetWorld(), FocusingCrosshairsClass);
	CrosshairsWidget->AddToViewport();

	//플레이어 UI띄우기
	PlayerUIWidget = CreateWidget<UPA_Player_UI>(GetWorld(), PlayerUIWidgetClass);
	//스킬쿨타임 설정
	PlayerUIWidget->BindPlayerStat(CharacterStat);
	PlayerUIWidget->AddToViewport();
}

// Called every frame
void APA_Player_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//모으기 중이면 모으는 시간을 측정함
	if (IsCharging) ChargingTime += DeltaTime;

	//폭탄 궤적
	if (OnBoomTrajectory)
	{
		OnTrajectory_Boom();
		Boom_Sphere->SetVisibility(true);
	}
	else Boom_Sphere->SetVisibility(false);

	//화살비 데칼
	if (OnArrowRainDecal)
	{
		OnDecal_ArrowRain();
		ArrowRain_Decal->SetVisibility(true);
	}
	else
	{
		ArrowRain_Decal->SetVisibility(false);
	}
}

// Called to bind functionality to input
void APA_Player_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//액션
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//점프
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &APA_Player_PlayerCharacter::StartJump);
	//일반 공격
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Focus);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &APA_Player_PlayerCharacter::Fire);
	//스킬 ( 오른쪽 마우스 클릭 )
	PlayerInputComponent->BindAction(TEXT("Skill_RightMouseClick"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_RightMouseClick);
	//스킬 ( Q )
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_Q_Focus);
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Released, this, &APA_Player_PlayerCharacter::Skill_Q_Fire);
	// 스킬 ( E )
	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_E);
	// 스킬 ( R )
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_R);
	// 스킬 ( Shift )
	PlayerInputComponent->BindAction(TEXT("Skill_Shift"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_Shift);
	// 스킬  ( G )
	PlayerInputComponent->BindAction(TEXT("Skill_G"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_G);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//축 매핑
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//이동
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APA_Player_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APA_Player_PlayerCharacter::MoveRight);
	//시야
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APA_Player_PlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APA_Player_PlayerCharacter::Turn);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

float APA_Player_PlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Player : %s took Damage : %f"),
		*GetName(), FinalDamage));	//temp

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

//점프
void APA_Player_PlayerCharacter::StartJump()
{
	bPressedJump = true;
}
void APA_Player_PlayerCharacter::StopJump()
{
	bPressedJump = false;
}
//일반 공격
void APA_Player_PlayerCharacter::Focus()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true) return;
	if (SkillState == ESkillState::RightMouse || SkillState == ESkillState::Idle)
	{
		//상태 변경
		SkillState = ESkillState::LeftMouse;

		//모으기 true
		IsCharging = true;
	}
}
void APA_Player_PlayerCharacter::Fire()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true || SkillState != ESkillState::LeftMouse) return;

	//상태 변경
	CanFire = false;

	//조준 관련
	FindImpactPoint();

	//몽타주 재생
	PlayerAnim->PlayFireMontage();

	//1초 이상 모았는지
	if (ChargingTime >= 1.0f)
	{
		//강화 화살 소환
		auto ArrowObject = GetWorld()->SpawnActor<APA_Object_PlayerArrow>(AdvancedArrow, ArrowSpawnLocation, ArrowSpawnRotator);
		if (ArrowObject != nullptr)
		{
			ArrowObject->IsAdvanced = true;
			ArrowObject->ArrowDamage = CharacterStat->GetAttackDamage();
		}
	}
	else
	{
		//일반 화살 소환
		auto ArrowObject = GetWorld()->SpawnActor<APA_Object_PlayerArrow>(Arrow, ArrowSpawnLocation, ArrowSpawnRotator);
		if (ArrowObject != nullptr)
		{
			ArrowObject->IsAdvanced = false;
			ArrowObject->ArrowDamage = CharacterStat->GetAttackDamage();
		}
	}

	//모으기 설정 초기화
	IsCharging = false;
	ChargingTime = 0.0f;
}
//스킬 ( 오른쪽 마우스 클릭 )
void APA_Player_PlayerCharacter::Skill_RightMouseClick()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true) return;

	if (IsFocusing)
	{
		//카메라 변경
		FocusCamera->SetActive(false);
		Camera->SetActive(true);

		//이동속도 복구 temp
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;

		//조준점 세팅
		SetCrosshairs(IsFocusing);

		//상태 변경
		SkillState = ESkillState::Idle;
	}
	else
	{
		//Idle상태에서만 작동한다
		if (SkillState != ESkillState::Idle) return;

		//카메라 변경
		Camera->SetActive(false);
		FocusCamera->SetActive(true);

		//이동속도 감소 temp
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;

		//조준점 세팅
		SetCrosshairs(IsFocusing);

		//상태 변경
		SkillState = ESkillState::RightMouse;
	}
}
//스킬 ( Q )
void APA_Player_PlayerCharacter::Skill_Q_Focus()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveQ())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaQ)) return;

	//상태 변경
	SkillState = ESkillState::Q;

	//폭탄 궤적과 예상 범위 On
	OnBoomTrajectory = true;
}
void APA_Player_PlayerCharacter::Skill_Q_Fire()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true || SkillState != ESkillState::Q) return;

	//마나 소비
	CharacterStat->UseMana(PlayerUIWidget->ManaQ);

	//상태 변경
	CanFire = false;
	
	//폭탄 궤적과 예상 범위 Off
	OnBoomTrajectory = false;

	//폭탄이 나갈 위치 구하는 코드
	FName SocketName = FName(TEXT("BowEmitterSocket"));
	FVector EmitterLocation = GetMesh()->GetSocketLocation(SocketName);
	FRotator EmitterRotation = GetMesh()->GetSocketRotation(SocketName);

	//액터 소환
	auto BoomObject = GetWorld()->SpawnActor<APA_Player_Skill_Boom>(Boom, EmitterLocation, EmitterRotation);
	BoomObject->SetBoomProperty(CharacterStat->GetLevel());

	//몽타주 재생
	PlayerAnim->PlaySpellMontage();

	//쿨타임
	PlayerUIWidget->ActiveSkillSlotQ();
}
void APA_Player_PlayerCharacter::OnTrajectory_Boom()
{
	//폭탄 오브젝트 생성 후, 스피드와 반지름을 얻어온다
	auto BoomObject = Cast<APA_Player_Skill_Boom>(Boom->GetDefaultObject());
	float BoomSpeed = BoomObject->Speed;
	float BoomRadius = BoomObject->Radius;

	//폭탄이 나갈 소켓의 이름
	FName SocketName("BowEmitterSocket");
	//폭탄의 출발 위치 설정
	FVector EmitterLocation = GetMesh()->GetSocketLocation(SocketName);
	//폭탄 속도 설정
	FVector TrajectoryLaunuchVelocity = UKismetMathLibrary::GetForwardVector(GetMesh()->GetSocketRotation(SocketName)) * BoomSpeed;
	//Trace 검사에 적용할 콜리전 설정 ( 배열 )
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	TEnumAsByte<EObjectTypeQuery> Destructible = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible);
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	TEnumAsByte<EObjectTypeQuery> Enemy = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);
	ObjectTypes.Add(Destructible);
	ObjectTypes.Add(Pawn);
	ObjectTypes.Add(Enemy);
	//Trace 검사에 제외할 액터 목록 ( 배열 )
	TArray<TObjectPtr<AActor>> IgnoreActor;
	IgnoreActor.Add(this);

	//PredictProjectilePath에 쓰일 파라미터 설정
	FPredictProjectilePathParams PredictParams;
	FPredictProjectilePathResult PredictResult;
	PredictParams.StartLocation = EmitterLocation;						//폭탄의 출발 위치
	PredictParams.LaunchVelocity = TrajectoryLaunuchVelocity;			//폭탄의 속도
	PredictParams.ObjectTypes = ObjectTypes;							//폭탄이 탐지할 콜리전
	PredictParams.DrawDebugTime = 0.0f;									//디버그 공모양의 잔상이 남는 시간 ( 디버그 타입 ForDuration 必 )
	PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;	//디버그 타입 설정 ( 한번만, 지정된 시간동안, 영원히 )
	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();			//중력 설정
	PredictParams.SimFrequency = 30.0f;									//디버그 빈도 설정
	PredictParams.MaxSimTime = 5.0f;									//디버그 타임 설정 ( 지정된 시간동안의 궤적만 디버그함 )
	PredictParams.ProjectileRadius = BoomRadius;						//디버그 반지름
	PredictParams.bTraceWithChannel = true;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ActorsToIgnore = IgnoreActor;

	//궤적 생성
	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

	//궤적 끝 부분 위치
	FVector DestinationVector = PredictResult.LastTraceDestination.Location;

	//폭탄 범위 머티리얼 생성
	Boom_Sphere->SetWorldLocation(DestinationVector);
}
//스킬 ( E )
void APA_Player_PlayerCharacter::Skill_E()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveE())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaE)) return;

	//마나 소비
	CharacterStat->UseMana(PlayerUIWidget->ManaE);

	//상태 변경
	CanFire = false;
	SkillState = ESkillState::E;

	//임팩트 포인트 구하는 함수
	FindImpactPoint();

	//액터 소환
	auto BlockObject = GetWorld()->SpawnActor<APA_Player_Skill_Block>(Block, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(NormalVector));
	BlockObject->SetBlockProperty(CharacterStat->GetLevel());
	BlockObject->BlockRiseUp();

	//몽타주 재생
	PlayerAnim->PlaySpellMontage();

	//쿨타임
	PlayerUIWidget->ActiveSkillSlotE();
}
//스킬 ( R )
void APA_Player_PlayerCharacter::Skill_R()
{
	if (!PlayerUIWidget->GetIsActiveR())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}

	if (OnArrowRainDecal)
	{
		//시전

		//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
		if (CanFire != true || SkillState != ESkillState::R) return;

		//마나 소비
		CharacterStat->UseMana(PlayerUIWidget->ManaR);

		//상태 변경
		CanFire = false;

		//데칼 off
		OnArrowRainDecal = false;

		//소환
		auto ArrowRainObject = GetWorld()->SpawnActor<APA_Player_Skill_ArrowRain>(ArrowRain, ImpactPoint, FRotator::ZeroRotator);
		ArrowRainObject->SetArrowRainProperty(CharacterStat->GetLevel());
		ArrowRainObject->SetArrowRainDamage(CharacterStat->GetAttackDamage());

		//몽타주 재생
		PlayerAnim->PlaySpellMontage();

		//쿨타임
		PlayerUIWidget->ActiveSkillSlotR();
	}
	else
	{
		//조준

		//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
		if (CanFire != true || SkillState != ESkillState::Idle) return;
		if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaR)) return;;

		//상태 변경
		SkillState = ESkillState::R;

		OnArrowRainDecal = true;
	}
}
void APA_Player_PlayerCharacter::OnDecal_ArrowRain()
{
	//임팩트 포인트
	FindImpactPoint();

	//위치 조정
	ArrowRain_Decal->SetWorldLocation(ImpactPoint);
}
//스킬 ( Shift )
void APA_Player_PlayerCharacter::Skill_Shift()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveShift())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaShift)) return;

	//마나 소비
	CharacterStat->UseMana(PlayerUIWidget->ManaShift);

	//방향 구하기
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//마찰력 0
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;

	//파티클 소환
	auto DashObject = GetWorld()->SpawnActor<APA_Player_Skill_Dash>(Dash, GetActorLocation(), GetActorRotation());
	DashObject->SetDashProperty(CharacterStat->GetLevel());

	//돌진 temp
	LaunchCharacter(Direction * DashObject->Speed, true, true);

	//상태변경
	SkillState = ESkillState::Shift;

	//타이머 설정 ( 일정 시간 후에 대시가 끝나는 함수 실행 )
	GetWorldTimerManager().SetTimer(DashHandle, FTimerDelegate::CreateLambda([this]()->void {
		//Stop Dash
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		SkillState = ESkillState::Idle;
		}),0.1f, false);

	//파티클 소환
	GetWorld()->SpawnActor<APA_Player_Skill_Dash>(Dash, GetActorLocation(), GetActorRotation());

	//쿨타임
	PlayerUIWidget->ActiveSkillSlotShift();
}
//스킬 ( G )
void APA_Player_PlayerCharacter::Skill_G()
{
	//공격할 수 없는 상황이면 이 함수가 발동되지 않는다.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveG())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaG)) return;

	if (TeleportObject)
	{
		//마나 소비
		CharacterStat->UseMana(PlayerUIWidget->ManaG);

		//텔레포트 시전
		TeleportTo(TeleportObject->GetActorLocation(), GetActorRotation());

		//오브젝트 삭제
		TeleportObject->Destroy();
		TeleportObject = nullptr;

		//쿨타임
		PlayerUIWidget->ActiveSkillSlotG();
	}
	else
	{
		//프로퍼티 변경
		CanFire = false;				//공격할 수 없는 상태로 변경

		//몽타주 재생
		FindImpactPoint();

		//몽타주 재생
		PlayerAnim->PlaySpellMontage();

		//액터 소환 후, 액터 설정
		ArrowSpawnLocation += GetActorForwardVector() * 50;		//활보다 좀 더 앞에서 나가도록 설정
		TeleportObject = GetWorld()->SpawnActor<APA_Player_Skill_Teleport>(Teleport, ArrowSpawnLocation, ArrowSpawnRotator);
		TeleportObject->SetSkill(CharacterStat->GetLevel());
		TeleportObject->OnDestroy.AddLambda([this]()->void { TeleportObject = nullptr; });
	}
}
//목표점 조정 함수
void APA_Player_PlayerCharacter::FindImpactPoint()
{
	FName SocktName(TEXT("BowEmitterSocket"));		//활 이미터의 소켓 이름
	//Trace Start Location
	FVector CrosshairForwardVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();
	CrosshairWorldLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation() +
		(CrosshairForwardVector * FVector(200.0f, 200.0f, 200.0f));	//카메라가 벽에 밀착되어 있으면 밀착되어 있는 벽을 목표로 잡는 것 방지
	//Trace End Location
	ImpactPoint = CrosshairWorldLocation + (CrosshairForwardVector * FVector(15000.0f, 15000.0f, 15000.0f));

	FHitResult OutHit;						//Hit결과를 담을 변수
	FCollisionQueryParams IgnoreOwner = FCollisionQueryParams::DefaultQueryParam;	//충돌체 정보를 담을 변수
	IgnoreOwner.AddIgnoredActor(GetOwner());//충돌체 정보에 무시할 충돌체 정보를 기입한다

	//활에서 목표지점까지 회전율을 설정하여 화살이 올바른 곳에 피격되도록하는 함수
	GetWorld()->LineTraceSingleByChannel(
		OutHit,									//Hit결과를 담을 변수
		CrosshairWorldLocation,					//Trace 시작 위치
		ImpactPoint,							//Trace 끝 위치
		ECollisionChannel::ECC_Visibility,		//라인 트레이스의 콜리전 설정
		IgnoreOwner								//추적을 도울 추가 충돌체 설정 ( 여기서는 무시할 충돌체를 추가함 )
	);

	bIsHitByTrace = OutHit.bBlockingHit;
	if (bIsHitByTrace) ImpactPoint = OutHit.ImpactPoint;
	ArrowSpawnLocation = GetMesh()->GetSocketTransform(SocktName).GetLocation();
	ArrowSpawnRotator = UKismetMathLibrary::MakeRotFromX(ImpactPoint - ArrowSpawnLocation);

	//법선 벡터
	NormalVector = OutHit.Normal;
}

//앞, 뒤 이동
void APA_Player_PlayerCharacter::MoveForward(float NewAxisValue)
{
	//컨트롤러의 Yaw만큼을 회전 시킨 회전행렬을 기준 삼은 X벡터로 이동
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}
//우, 좌 이동
void APA_Player_PlayerCharacter::MoveRight(float NewAxisValue)
{
	//컨트롤러의 Yaw만큼을 회전 시킨 회전행렬을 기준 삼은 Y벡터로 이동
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
}
//시야 ( 위, 아래 )
void APA_Player_PlayerCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
//시야 ( 좌, 우 )
void APA_Player_PlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

//조준점
void APA_Player_PlayerCharacter::SetCrosshairs(bool bIsFocusing)
{
	if (CrosshairsWidget && FocusingCrosshairsWidget)
	{
		if (bIsFocusing)
		{
			FocusingCrosshairsWidget->RemoveFromParent();
			CrosshairsWidget->AddToViewport();
			IsFocusing = false;
		}
		else
		{
			CrosshairsWidget->RemoveFromParent();
			FocusingCrosshairsWidget->AddToViewport();
			IsFocusing = true;
		}
	}
}

//사망
void APA_Player_PlayerCharacter::PlayerDead()
{
	PlayerAnim->IsDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		//죽으면 UI뜨게하는 함수 구현
		Destroy();							//이 액터를 파괴한다
		}), DeadTimer, false);
}

//레벨업
void APA_Player_PlayerCharacter::LevelUp_fun()
{
	//NULL체크 후 플레이어 UI를 레벨업할 시에 변경하는 코드
	if (PlayerUIWidget != nullptr) PlayerUIWidget->SetSkillCoolTime(CharacterStat->GetLevel());
	else UE_LOG(LogTemp, Error, TEXT("PlayerUIWidget is nullptr"));

	//NULL체크 후 LevelUpWidgetComponent를 레벨업할 시에 재생하는 코드
	if (LevelUpWidgetComponent != nullptr)
	{
		auto LevelUpWidgetObject = Cast<UPA_Player_LevelUpWidget>(LevelUpWidgetComponent->GetWidget());
		LevelUpWidgetObject->PlayLevelUpWidgetAnimation();
	}
	else UE_LOG(LogTemp, Error, TEXT("LevelUpWidgetComponent is nullptr"));

	//파티클
	LevelUpNiagara->Activate();
}