// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_Object_PlayerArrow.generated.h"

UCLASS()
class PROJECTARCHER_API APA_Object_PlayerArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APA_Object_PlayerArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	USceneComponent* DefaultScene;								//씬 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	UStaticMeshComponent* Arrow;								//화살 메시
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	class UBoxComponent* Collision;								//충돌체
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	class UProjectileMovementComponent* ProjectileMovement;		//발사체 무브먼트

	//강화 화살인지 확인용 변수
	bool IsAdvanced = false;

	//치명타 확인용 변수
	bool IsCritical = false;

	//화살 대미지
	float ArrowDamage = 0.0f;

private:
	//파티클 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETC", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* HitParticleSystem;					//화살 히트 파티클 시스템 ( 블프에서 설정함 )

	UFUNCTION()
	void OnArrowBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	//화살의 오버랩이 시작되었을 때

};
