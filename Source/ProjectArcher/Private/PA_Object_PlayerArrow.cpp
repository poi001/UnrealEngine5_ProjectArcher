// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Object_PlayerArrow.h"
#include "PA_Player_PlayerController.h"
#include "PA_Player_PlayerStat.h"

#include "Components/BoxComponent.h"					//박스콜리전
#include "GameFramework/ProjectileMovementComponent.h"	//발사체무브먼트, 대미지
#include "Kismet/GameplayStatics.h"						//SpawnEmitterAtLocation
#include "Particles/ParticleSystem.h"					//파티클 시스템
#include "Particles/ParticleSystemComponent.h"			//파티클 시스템 컴포넌트


// Sets default values
APA_Object_PlayerArrow::APA_Object_PlayerArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//컴포넌트
	DefaultScene = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULTSCENE"));
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARROW"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLISION"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILEMOVEMENT"));

	//컴포넌트 부착
	RootComponent = DefaultScene;
	Arrow->SetupAttachment(DefaultScene);
	Collision->SetupAttachment(Arrow);

	//컴포넌트 오버랩 설정
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
	if (OtherActor && (OtherActor != this) && OtherComp)	//오버랩 되는 액터와 컴포넌트가 존재하고 자기자신이 아닌지 확인
	{
		ProjectileMovement->StopMovementImmediately();								//화살의 이동을 멈춘다
		ProjectileMovement->ProjectileGravityScale = 0.0f;							//화살의 중력스캐일을 0으로 설정해 공중에 뜨게 한다
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);	//맞은 액터에 붙인다. (규칙은 월드 트랜스폼을 유지하도록)
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);				//화살의 콜리전을 NoCollision으로 설정한다
		//피격 파티클을 화살이 위치한 부분에 소환한다 ( 파티클시스템은 블루프린트에서 설정하였다 )
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, Arrow->GetComponentLocation());

		//데미지타입 클래스
		TSubclassOf<UDamageType> DamageTypeClass;
		//플레이어 컨트롤러
		auto PlayerControllerObject = Cast<APA_Player_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		//헤드샷 여부
		if (OtherComp->GetName() == FString("HeadCollision"))
		{
			//치명타 확인
			IsCritical = true;

			//강화화살 확인
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
			//치명타 확인
			IsCritical = false;

			//강화화살 확인
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