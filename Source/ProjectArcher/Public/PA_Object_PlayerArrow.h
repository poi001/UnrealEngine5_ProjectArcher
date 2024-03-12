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

	//������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	USceneComponent* DefaultScene;								//�� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	UStaticMeshComponent* Arrow;								//ȭ�� �޽�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	class UBoxComponent* Collision;								//�浹ü
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	class UProjectileMovementComponent* ProjectileMovement;		//�߻�ü �����Ʈ

	//��ȭ ȭ������ Ȯ�ο� ����
	bool IsAdvanced = false;

	//ġ��Ÿ Ȯ�ο� ����
	bool IsCritical = false;

	//ȭ�� �����
	float ArrowDamage = 0.0f;

private:
	//��ƼŬ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETC", Meta = (AllowPrivateAccess = true))
	class UParticleSystem* HitParticleSystem;					//ȭ�� ��Ʈ ��ƼŬ �ý��� ( �������� ������ )

	UFUNCTION()
	void OnArrowBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	//ȭ���� �������� ���۵Ǿ��� ��

};
