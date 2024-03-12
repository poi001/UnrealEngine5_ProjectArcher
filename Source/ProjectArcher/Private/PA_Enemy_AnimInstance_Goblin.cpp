// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_AnimInstance_Goblin.h"


UPA_Enemy_AnimInstance_Goblin::UPA_Enemy_AnimInstance_Goblin()
{
	//°ø°Ý ¸ùÅ¸ÁÖ
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Game/UndeadPack/EnemyGoblin/Animations/Montage_Attack.Montage_Attack"));
	if (ATTACK_MONTAGE.Succeeded()) AttackMontage = ATTACK_MONTAGE.Object;
}

void UPA_Enemy_AnimInstance_Goblin::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
	IsAttack = true;
}