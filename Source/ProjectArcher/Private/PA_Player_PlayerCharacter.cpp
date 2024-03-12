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
#include "GameFramework/SpringArmComponent.h"			//��������
#include "Camera/CameraComponent.h"						//ī�޶�
#include "Components/CapsuleComponent.h"				//ĸ��������Ʈ
#include "GameFramework/CharacterMovementComponent.h"	//ĳ���͹����Ʈ
#include "Blueprint/UserWidget.h"						//����
#include "Kismet/KismetMathLibrary.h"					//MakeRotFromX
#include "Components/DecalComponent.h"					//��Į ������Ʈ
#include "Components/WidgetComponent.h"					//���� ������Ʈ
#include "Particles/ParticleSystem.h"					//��ƼŬ �ý���
#include "NiagaraFunctionLibrary.h"						//���̾ư��� �Լ�
#include "NiagaraComponent.h"							//���̾ư��� ������Ʈ


// Sets default values
APA_Player_PlayerCharacter::APA_Player_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������Ʈ ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));				//CameraBoom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));						//Camera
	FocusCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOCUSCAMERA"));			//FocusCamera
	CharacterStat = CreateDefaultSubobject<UPA_Player_PlayerStat>(TEXT("CHARACTERSTAT"));	//����
	Boom_Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOOM_SPHERE"));		//��ź �ݰ� �޽�
	ArrowRain_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("ARROWRAIN_DECAL"));		//ȭ��� �ݰ� ��Į
	LevelUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LEVELUP"));		//������ �� ����� ���� ������Ʈ
	LevelUpNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LEVELUPNIAGARA"));		//������ ���̾ư���

	//������Ʈ ����
	SpringArm->SetupAttachment(GetCapsuleComponent());				//CameraBoom
	Camera->SetupAttachment(SpringArm);								//Camera
	FocusCamera->SetupAttachment(GetMesh());						//FocusCamera
	LevelUpWidgetComponent->SetupAttachment(GetMesh());				//������ ����
	LevelUpNiagara->SetupAttachment(GetCapsuleComponent());

	//������Ʈ ����
	Boom_Sphere->SetVisibility(false);
	ArrowRain_Decal->SetVisibility(false);
	
	//�÷��̾� ��Ʈ�ѷ� ȸ�� ���� ( ��Ʈ�ѷ��� ���缭 ĳ���Ͱ� ȸ���ϴ� ���� )
	bUseControllerRotationPitch = false;										// ��Ʈ�ѷ��� Pitch�� ��� ����
	bUseControllerRotationYaw = true;											// ��Ʈ�ѷ��� Yaw�� ��� ����
	bUseControllerRotationRoll = false;											// ��Ʈ�ѷ��� Roll�� ��� ����

	//ī�޶� ���� ( SpringArm )
	SpringArm->TargetArmLength = 750;											// ������ ���� ����
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));				// ������ ���� ȸ������ �����Ѵ�
	SpringArm->SocketOffset = FVector(0.0f, 120.0f, 75.0f);						// ������ ���� �� �κ��� �����Ѵ�
	SpringArm->bDoCollisionTest = true;											// ī�޶� ���� �浹�ϴ����� ���� ����
	SpringArm->bUsePawnControlRotation = true;									// ���������� ���� �����̼� ���� ����� ������ ����
	SpringArm->bInheritPitch = true;											// ������ ���� ȸ������ ��ӹ޴´�. ( ī�޶� )
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	FocusCamera->bUsePawnControlRotation = true;								// ī�޶� ���� �����̼� ���� ����� ������ ����
	Camera->SetActive(true);													//Camera�� �⺻ ī�޶�� ����
	FocusCamera->SetActive(false);												//FocusCamera�� �⺻ ī�޶�� �������� ����

	//ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = false;					// ���� �ִ� ���� ������ ĳ���Ͱ� �Ĵ� ��
	GetCharacterMovement()->JumpZVelocity = 600.0f;								// ���� ����
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;								// �̵� �ӵ�

	//�⺻ ���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MANNEQUIN(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (MANNEQUIN.Succeeded()) GetMesh()->SetSkeletalMesh(MANNEQUIN.Object);
	//���̷�Ż �޽� ��ġ �� ȸ�� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//�ִϸ��̼� ��带 �ִϸ��̼� �������Ʈ�� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//�⺻ �ִ� �ν��Ͻ� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM(TEXT("/Game/Character/Player/AnimBP/AnimBP_PlayerCharacter.AnimBP_PlayerCharacter_C"));
	if (PLAYER_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);

	//ȭ��
	static ConstructorHelpers::FClassFinder<APA_Object_PlayerArrow> ARROW(TEXT("/Game/Object/Arrow/BP/BP_Arrow.BP_Arrow_C"));
	if (ARROW.Succeeded()) Arrow = ARROW.Class;								//ȭ��
	static ConstructorHelpers::FClassFinder<APA_Object_PlayerArrow> ADVANCEDARROW(TEXT("/Game/Object/Arrow/BP/BP_AdvancedArrow.BP_AdvancedArrow_C"));
	if (ADVANCEDARROW.Succeeded()) AdvancedArrow = ADVANCEDARROW.Class;		//��ȭ ȭ��

	//����
	static ConstructorHelpers::FClassFinder<UUserWidget> CROSSHAIR(TEXT("/Game/Widget/HUD/WBP_Crosshairs.WBP_Crosshairs_C"));
	if (CROSSHAIR.Succeeded()) CrosshairsClass = CROSSHAIR.Class;							//�Ϲ� ������
	static ConstructorHelpers::FClassFinder<UUserWidget> FOCUSINGCROSSHAIR(TEXT("/Game/Widget/HUD/WBP_Crosshairs_Focus.WBP_Crosshairs_Focus_C"));
	if (FOCUSINGCROSSHAIR.Succeeded()) FocusingCrosshairsClass = FOCUSINGCROSSHAIR.Class;	//���� ������
	static ConstructorHelpers::FClassFinder<UPA_Player_UI> PLAYER_UI_C(TEXT("/Game/Widget/UI/WBP_UI.WBP_UI_C"));
	if (PLAYER_UI_C.Succeeded()) PlayerUIWidgetClass = PLAYER_UI_C.Class;					//�÷��̾� UI( HP, MP, EXP, Skill )

	LevelUpWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);					//������ ������ �׻� ��ũ���� ���̵��� ����
	static ConstructorHelpers::FClassFinder<UPA_Player_LevelUpWidget>LEVELUP_WIDGET(TEXT("/Game/Widget/LevelUp/WBP_LevelUp.WBP_LevelUp_C"));
	if (LEVELUP_WIDGET.Succeeded())
	{
		LevelUpWidgetComponent->SetWidgetClass(LEVELUP_WIDGET.Class);						//�⺻ Ŭ������ ����
		LevelUpWidgetComponent->SetDrawSize(FVector2D(256.0f, 75.0f));						//������ ũ�� ����
	}

	//��ų
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
	
	//������Ƽ
	IsFocusing = false;
}

void APA_Player_PlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//�ִ� �ν��Ͻ��� PlayerAnim������ ���� �ִ´�
	PlayerAnim = Cast<UPA_Player_PlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (PlayerAnim == nullptr) UE_LOG(LogTemp, Error, TEXT("PlayerAnimInstance is nullptr"));	//nullüũ

	//�߻� ��������Ʈ
	PlayerAnim->OnResetCombo.AddLambda([this]()->void { 
		CanFire = true;
		SkillState = ESkillState::Idle;
		});

	//ĳ���� ���� ��, ��������Ʈ
	CharacterStat->OnHPIsZeroDelegate.AddUObject(this, &APA_Player_PlayerCharacter::PlayerDead);

	//�������� �� ��, ��ų���� ������ ������Ʈ ��������Ʈ
	CharacterStat->OnLevelUpDelegate.AddUObject(this, &APA_Player_PlayerCharacter::LevelUp_fun);
}

// Called when the game starts or when spawned
void APA_Player_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//������ ����
	CrosshairsWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairsClass);
	FocusingCrosshairsWidget = CreateWidget<UUserWidget>(GetWorld(), FocusingCrosshairsClass);
	CrosshairsWidget->AddToViewport();

	//�÷��̾� UI����
	PlayerUIWidget = CreateWidget<UPA_Player_UI>(GetWorld(), PlayerUIWidgetClass);
	//��ų��Ÿ�� ����
	PlayerUIWidget->BindPlayerStat(CharacterStat);
	PlayerUIWidget->AddToViewport();
}

// Called every frame
void APA_Player_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//������ ���̸� ������ �ð��� ������
	if (IsCharging) ChargingTime += DeltaTime;

	//��ź ����
	if (OnBoomTrajectory)
	{
		OnTrajectory_Boom();
		Boom_Sphere->SetVisibility(true);
	}
	else Boom_Sphere->SetVisibility(false);

	//ȭ��� ��Į
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

	//�׼�
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//����
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::StartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &APA_Player_PlayerCharacter::StartJump);
	//�Ϲ� ����
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Focus);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &APA_Player_PlayerCharacter::Fire);
	//��ų ( ������ ���콺 Ŭ�� )
	PlayerInputComponent->BindAction(TEXT("Skill_RightMouseClick"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_RightMouseClick);
	//��ų ( Q )
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_Q_Focus);
	PlayerInputComponent->BindAction(TEXT("Skill_Q"), EInputEvent::IE_Released, this, &APA_Player_PlayerCharacter::Skill_Q_Fire);
	// ��ų ( E )
	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_E);
	// ��ų ( R )
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_R);
	// ��ų ( Shift )
	PlayerInputComponent->BindAction(TEXT("Skill_Shift"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_Shift);
	// ��ų  ( G )
	PlayerInputComponent->BindAction(TEXT("Skill_G"), EInputEvent::IE_Pressed, this, &APA_Player_PlayerCharacter::Skill_G);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�� ����
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//�̵�
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APA_Player_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APA_Player_PlayerCharacter::MoveRight);
	//�þ�
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

//����
void APA_Player_PlayerCharacter::StartJump()
{
	bPressedJump = true;
}
void APA_Player_PlayerCharacter::StopJump()
{
	bPressedJump = false;
}
//�Ϲ� ����
void APA_Player_PlayerCharacter::Focus()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true) return;
	if (SkillState == ESkillState::RightMouse || SkillState == ESkillState::Idle)
	{
		//���� ����
		SkillState = ESkillState::LeftMouse;

		//������ true
		IsCharging = true;
	}
}
void APA_Player_PlayerCharacter::Fire()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true || SkillState != ESkillState::LeftMouse) return;

	//���� ����
	CanFire = false;

	//���� ����
	FindImpactPoint();

	//��Ÿ�� ���
	PlayerAnim->PlayFireMontage();

	//1�� �̻� ��Ҵ���
	if (ChargingTime >= 1.0f)
	{
		//��ȭ ȭ�� ��ȯ
		auto ArrowObject = GetWorld()->SpawnActor<APA_Object_PlayerArrow>(AdvancedArrow, ArrowSpawnLocation, ArrowSpawnRotator);
		if (ArrowObject != nullptr)
		{
			ArrowObject->IsAdvanced = true;
			ArrowObject->ArrowDamage = CharacterStat->GetAttackDamage();
		}
	}
	else
	{
		//�Ϲ� ȭ�� ��ȯ
		auto ArrowObject = GetWorld()->SpawnActor<APA_Object_PlayerArrow>(Arrow, ArrowSpawnLocation, ArrowSpawnRotator);
		if (ArrowObject != nullptr)
		{
			ArrowObject->IsAdvanced = false;
			ArrowObject->ArrowDamage = CharacterStat->GetAttackDamage();
		}
	}

	//������ ���� �ʱ�ȭ
	IsCharging = false;
	ChargingTime = 0.0f;
}
//��ų ( ������ ���콺 Ŭ�� )
void APA_Player_PlayerCharacter::Skill_RightMouseClick()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true) return;

	if (IsFocusing)
	{
		//ī�޶� ����
		FocusCamera->SetActive(false);
		Camera->SetActive(true);

		//�̵��ӵ� ���� temp
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;

		//������ ����
		SetCrosshairs(IsFocusing);

		//���� ����
		SkillState = ESkillState::Idle;
	}
	else
	{
		//Idle���¿����� �۵��Ѵ�
		if (SkillState != ESkillState::Idle) return;

		//ī�޶� ����
		Camera->SetActive(false);
		FocusCamera->SetActive(true);

		//�̵��ӵ� ���� temp
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;

		//������ ����
		SetCrosshairs(IsFocusing);

		//���� ����
		SkillState = ESkillState::RightMouse;
	}
}
//��ų ( Q )
void APA_Player_PlayerCharacter::Skill_Q_Focus()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveQ())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaQ)) return;

	//���� ����
	SkillState = ESkillState::Q;

	//��ź ������ ���� ���� On
	OnBoomTrajectory = true;
}
void APA_Player_PlayerCharacter::Skill_Q_Fire()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true || SkillState != ESkillState::Q) return;

	//���� �Һ�
	CharacterStat->UseMana(PlayerUIWidget->ManaQ);

	//���� ����
	CanFire = false;
	
	//��ź ������ ���� ���� Off
	OnBoomTrajectory = false;

	//��ź�� ���� ��ġ ���ϴ� �ڵ�
	FName SocketName = FName(TEXT("BowEmitterSocket"));
	FVector EmitterLocation = GetMesh()->GetSocketLocation(SocketName);
	FRotator EmitterRotation = GetMesh()->GetSocketRotation(SocketName);

	//���� ��ȯ
	auto BoomObject = GetWorld()->SpawnActor<APA_Player_Skill_Boom>(Boom, EmitterLocation, EmitterRotation);
	BoomObject->SetBoomProperty(CharacterStat->GetLevel());

	//��Ÿ�� ���
	PlayerAnim->PlaySpellMontage();

	//��Ÿ��
	PlayerUIWidget->ActiveSkillSlotQ();
}
void APA_Player_PlayerCharacter::OnTrajectory_Boom()
{
	//��ź ������Ʈ ���� ��, ���ǵ�� �������� ���´�
	auto BoomObject = Cast<APA_Player_Skill_Boom>(Boom->GetDefaultObject());
	float BoomSpeed = BoomObject->Speed;
	float BoomRadius = BoomObject->Radius;

	//��ź�� ���� ������ �̸�
	FName SocketName("BowEmitterSocket");
	//��ź�� ��� ��ġ ����
	FVector EmitterLocation = GetMesh()->GetSocketLocation(SocketName);
	//��ź �ӵ� ����
	FVector TrajectoryLaunuchVelocity = UKismetMathLibrary::GetForwardVector(GetMesh()->GetSocketRotation(SocketName)) * BoomSpeed;
	//Trace �˻翡 ������ �ݸ��� ���� ( �迭 )
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
	//Trace �˻翡 ������ ���� ��� ( �迭 )
	TArray<TObjectPtr<AActor>> IgnoreActor;
	IgnoreActor.Add(this);

	//PredictProjectilePath�� ���� �Ķ���� ����
	FPredictProjectilePathParams PredictParams;
	FPredictProjectilePathResult PredictResult;
	PredictParams.StartLocation = EmitterLocation;						//��ź�� ��� ��ġ
	PredictParams.LaunchVelocity = TrajectoryLaunuchVelocity;			//��ź�� �ӵ�
	PredictParams.ObjectTypes = ObjectTypes;							//��ź�� Ž���� �ݸ���
	PredictParams.DrawDebugTime = 0.0f;									//����� ������� �ܻ��� ���� �ð� ( ����� Ÿ�� ForDuration �� )
	PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;	//����� Ÿ�� ���� ( �ѹ���, ������ �ð�����, ������ )
	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();			//�߷� ����
	PredictParams.SimFrequency = 30.0f;									//����� �� ����
	PredictParams.MaxSimTime = 5.0f;									//����� Ÿ�� ���� ( ������ �ð������� ������ ������� )
	PredictParams.ProjectileRadius = BoomRadius;						//����� ������
	PredictParams.bTraceWithChannel = true;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ActorsToIgnore = IgnoreActor;

	//���� ����
	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

	//���� �� �κ� ��ġ
	FVector DestinationVector = PredictResult.LastTraceDestination.Location;

	//��ź ���� ��Ƽ���� ����
	Boom_Sphere->SetWorldLocation(DestinationVector);
}
//��ų ( E )
void APA_Player_PlayerCharacter::Skill_E()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveE())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaE)) return;

	//���� �Һ�
	CharacterStat->UseMana(PlayerUIWidget->ManaE);

	//���� ����
	CanFire = false;
	SkillState = ESkillState::E;

	//����Ʈ ����Ʈ ���ϴ� �Լ�
	FindImpactPoint();

	//���� ��ȯ
	auto BlockObject = GetWorld()->SpawnActor<APA_Player_Skill_Block>(Block, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(NormalVector));
	BlockObject->SetBlockProperty(CharacterStat->GetLevel());
	BlockObject->BlockRiseUp();

	//��Ÿ�� ���
	PlayerAnim->PlaySpellMontage();

	//��Ÿ��
	PlayerUIWidget->ActiveSkillSlotE();
}
//��ų ( R )
void APA_Player_PlayerCharacter::Skill_R()
{
	if (!PlayerUIWidget->GetIsActiveR())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}

	if (OnArrowRainDecal)
	{
		//����

		//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
		if (CanFire != true || SkillState != ESkillState::R) return;

		//���� �Һ�
		CharacterStat->UseMana(PlayerUIWidget->ManaR);

		//���� ����
		CanFire = false;

		//��Į off
		OnArrowRainDecal = false;

		//��ȯ
		auto ArrowRainObject = GetWorld()->SpawnActor<APA_Player_Skill_ArrowRain>(ArrowRain, ImpactPoint, FRotator::ZeroRotator);
		ArrowRainObject->SetArrowRainProperty(CharacterStat->GetLevel());
		ArrowRainObject->SetArrowRainDamage(CharacterStat->GetAttackDamage());

		//��Ÿ�� ���
		PlayerAnim->PlaySpellMontage();

		//��Ÿ��
		PlayerUIWidget->ActiveSkillSlotR();
	}
	else
	{
		//����

		//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
		if (CanFire != true || SkillState != ESkillState::Idle) return;
		if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaR)) return;;

		//���� ����
		SkillState = ESkillState::R;

		OnArrowRainDecal = true;
	}
}
void APA_Player_PlayerCharacter::OnDecal_ArrowRain()
{
	//����Ʈ ����Ʈ
	FindImpactPoint();

	//��ġ ����
	ArrowRain_Decal->SetWorldLocation(ImpactPoint);
}
//��ų ( Shift )
void APA_Player_PlayerCharacter::Skill_Shift()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveShift())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaShift)) return;

	//���� �Һ�
	CharacterStat->UseMana(PlayerUIWidget->ManaShift);

	//���� ���ϱ�
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//������ 0
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;

	//��ƼŬ ��ȯ
	auto DashObject = GetWorld()->SpawnActor<APA_Player_Skill_Dash>(Dash, GetActorLocation(), GetActorRotation());
	DashObject->SetDashProperty(CharacterStat->GetLevel());

	//���� temp
	LaunchCharacter(Direction * DashObject->Speed, true, true);

	//���º���
	SkillState = ESkillState::Shift;

	//Ÿ�̸� ���� ( ���� �ð� �Ŀ� ��ð� ������ �Լ� ���� )
	GetWorldTimerManager().SetTimer(DashHandle, FTimerDelegate::CreateLambda([this]()->void {
		//Stop Dash
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		SkillState = ESkillState::Idle;
		}),0.1f, false);

	//��ƼŬ ��ȯ
	GetWorld()->SpawnActor<APA_Player_Skill_Dash>(Dash, GetActorLocation(), GetActorRotation());

	//��Ÿ��
	PlayerUIWidget->ActiveSkillSlotShift();
}
//��ų ( G )
void APA_Player_PlayerCharacter::Skill_G()
{
	//������ �� ���� ��Ȳ�̸� �� �Լ��� �ߵ����� �ʴ´�.
	if (CanFire != true || SkillState != ESkillState::Idle) return;
	if (!PlayerUIWidget->GetIsActiveG())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Ready Skill"));
		return;
	}
	if (!CharacterStat->ManaCheck(PlayerUIWidget->ManaG)) return;

	if (TeleportObject)
	{
		//���� �Һ�
		CharacterStat->UseMana(PlayerUIWidget->ManaG);

		//�ڷ���Ʈ ����
		TeleportTo(TeleportObject->GetActorLocation(), GetActorRotation());

		//������Ʈ ����
		TeleportObject->Destroy();
		TeleportObject = nullptr;

		//��Ÿ��
		PlayerUIWidget->ActiveSkillSlotG();
	}
	else
	{
		//������Ƽ ����
		CanFire = false;				//������ �� ���� ���·� ����

		//��Ÿ�� ���
		FindImpactPoint();

		//��Ÿ�� ���
		PlayerAnim->PlaySpellMontage();

		//���� ��ȯ ��, ���� ����
		ArrowSpawnLocation += GetActorForwardVector() * 50;		//Ȱ���� �� �� �տ��� �������� ����
		TeleportObject = GetWorld()->SpawnActor<APA_Player_Skill_Teleport>(Teleport, ArrowSpawnLocation, ArrowSpawnRotator);
		TeleportObject->SetSkill(CharacterStat->GetLevel());
		TeleportObject->OnDestroy.AddLambda([this]()->void { TeleportObject = nullptr; });
	}
}
//��ǥ�� ���� �Լ�
void APA_Player_PlayerCharacter::FindImpactPoint()
{
	FName SocktName(TEXT("BowEmitterSocket"));		//Ȱ �̹����� ���� �̸�
	//Trace Start Location
	FVector CrosshairForwardVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();
	CrosshairWorldLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation() +
		(CrosshairForwardVector * FVector(200.0f, 200.0f, 200.0f));	//ī�޶� ���� �����Ǿ� ������ �����Ǿ� �ִ� ���� ��ǥ�� ��� �� ����
	//Trace End Location
	ImpactPoint = CrosshairWorldLocation + (CrosshairForwardVector * FVector(15000.0f, 15000.0f, 15000.0f));

	FHitResult OutHit;						//Hit����� ���� ����
	FCollisionQueryParams IgnoreOwner = FCollisionQueryParams::DefaultQueryParam;	//�浹ü ������ ���� ����
	IgnoreOwner.AddIgnoredActor(GetOwner());//�浹ü ������ ������ �浹ü ������ �����Ѵ�

	//Ȱ���� ��ǥ�������� ȸ������ �����Ͽ� ȭ���� �ùٸ� ���� �ǰݵǵ����ϴ� �Լ�
	GetWorld()->LineTraceSingleByChannel(
		OutHit,									//Hit����� ���� ����
		CrosshairWorldLocation,					//Trace ���� ��ġ
		ImpactPoint,							//Trace �� ��ġ
		ECollisionChannel::ECC_Visibility,		//���� Ʈ���̽��� �ݸ��� ����
		IgnoreOwner								//������ ���� �߰� �浹ü ���� ( ���⼭�� ������ �浹ü�� �߰��� )
	);

	bIsHitByTrace = OutHit.bBlockingHit;
	if (bIsHitByTrace) ImpactPoint = OutHit.ImpactPoint;
	ArrowSpawnLocation = GetMesh()->GetSocketTransform(SocktName).GetLocation();
	ArrowSpawnRotator = UKismetMathLibrary::MakeRotFromX(ImpactPoint - ArrowSpawnLocation);

	//���� ����
	NormalVector = OutHit.Normal;
}

//��, �� �̵�
void APA_Player_PlayerCharacter::MoveForward(float NewAxisValue)
{
	//��Ʈ�ѷ��� Yaw��ŭ�� ȸ�� ��Ų ȸ������� ���� ���� X���ͷ� �̵�
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
}
//��, �� �̵�
void APA_Player_PlayerCharacter::MoveRight(float NewAxisValue)
{
	//��Ʈ�ѷ��� Yaw��ŭ�� ȸ�� ��Ų ȸ������� ���� ���� Y���ͷ� �̵�
	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
}
//�þ� ( ��, �Ʒ� )
void APA_Player_PlayerCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
//�þ� ( ��, �� )
void APA_Player_PlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

//������
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

//���
void APA_Player_PlayerCharacter::PlayerDead()
{
	PlayerAnim->IsDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		//������ UI�߰��ϴ� �Լ� ����
		Destroy();							//�� ���͸� �ı��Ѵ�
		}), DeadTimer, false);
}

//������
void APA_Player_PlayerCharacter::LevelUp_fun()
{
	//NULLüũ �� �÷��̾� UI�� �������� �ÿ� �����ϴ� �ڵ�
	if (PlayerUIWidget != nullptr) PlayerUIWidget->SetSkillCoolTime(CharacterStat->GetLevel());
	else UE_LOG(LogTemp, Error, TEXT("PlayerUIWidget is nullptr"));

	//NULLüũ �� LevelUpWidgetComponent�� �������� �ÿ� ����ϴ� �ڵ�
	if (LevelUpWidgetComponent != nullptr)
	{
		auto LevelUpWidgetObject = Cast<UPA_Player_LevelUpWidget>(LevelUpWidgetComponent->GetWidget());
		LevelUpWidgetObject->PlayLevelUpWidgetAnimation();
	}
	else UE_LOG(LogTemp, Error, TEXT("LevelUpWidgetComponent is nullptr"));

	//��ƼŬ
	LevelUpNiagara->Activate();
}