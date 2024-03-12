// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Object_PlayerArrow.h"
#include "PA_Player_PlayerController.h"
#include "PA_Player_PlayerStat.h"

#include "Components/BoxComponent.h"					//�ڽ��ݸ���
#include "GameFramework/ProjectileMovementComponent.h"	//�߻�ü�����Ʈ, �����
#include "Kismet/GameplayStatics.h"						//SpawnEmitterAtLocation
#include "Particles/ParticleSystem.h"					//��ƼŬ �ý���
#include "Particles/ParticleSystemComponent.h"			//��ƼŬ �ý��� ������Ʈ


// Sets default values
APA_Object_PlayerArrow::APA_Object_PlayerArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//������Ʈ
	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULTSCENE"));
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARROW"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLISION"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILEMOVEMENT"));

	//������Ʈ ����
	RootComponent = DefaultScene;
	Arrow->SetupAttachment(DefaultScene);
	Collision->SetupAttachment(Arrow);

	//������Ʈ ������ ����
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APA_Object_PlayerArrow::OnArrowBeginOverlap);
}

// Called when the game starts or when spawned
void APA_Object_PlayerArrow::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void APA_Object_PlayerArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APA_Object_PlayerArrow::OnArrowBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)	//������ �Ǵ� ���Ϳ� ������Ʈ�� �����ϰ� �ڱ��ڽ��� �ƴ��� Ȯ��
	{
		ProjectileMovement->StopMovementImmediately();								//ȭ���� �̵��� �����
		ProjectileMovement->ProjectileGravityScale = 0.0f;							//ȭ���� �߷½�ĳ���� 0���� ������ ���߿� �߰� �Ѵ�
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);	//���� ���Ϳ� ���δ�. (��Ģ�� ���� Ʈ�������� �����ϵ���)
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);				//ȭ���� �ݸ����� NoCollision���� �����Ѵ�
		//�ǰ� ��ƼŬ�� ȭ���� ��ġ�� �κп� ��ȯ�Ѵ� ( ��ƼŬ�ý����� �������Ʈ���� �����Ͽ��� )
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, Arrow->GetComponentLocation());

		//������Ÿ�� Ŭ����
		TSubclassOf<UDamageType> DamageTypeClass;
		//�÷��̾� ��Ʈ�ѷ�
		auto PlayerControllerObject = Cast<APA_Player_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		//��弦 ����
		if (OtherComp->GetName() == FString("HeadCollision"))
		{
			//ġ��Ÿ Ȯ��
			IsCritical = true;

			//��ȭȭ�� Ȯ��
			if (IsAdvanced)
			{
				UGameplayStatics::ApplyDamage(OtherActor, ArrowDamage * 2 * 1.75, PlayerControllerObject, this, DamageTypeClass);
			}
			else
			{
				UGameplayStatics::ApplyDamage(OtherActor, ArrowDamage * 1.75, PlayerControllerObject, this, DamageTypeClass);
			}
		}
		else
		{
			//ġ��Ÿ Ȯ��
			IsCritical = false;

			//��ȭȭ�� Ȯ��
			if (IsAdvanced)
			{
				UGameplayStatics::ApplyDamage(OtherActor, ArrowDamage * 2, PlayerControllerObject, this, DamageTypeClass);
			}
			else
			{
				UGameplayStatics::ApplyDamage(OtherActor, ArrowDamage, PlayerControllerObject, this, DamageTypeClass);
			}
		}
		Destroy();
	}
}