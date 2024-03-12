// Fill out your copyright notice in the Description page of Project Settings.


#include "PA_Enemy_AnimInstance_Zombie.h"


UPA_Enemy_AnimInstance_Zombie::UPA_Enemy_AnimInstance_Zombie()
{
	//°ø°Ý ¸ùÅ¸ÁÖ
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("/Game/UndeadPack/Zombie/Animations/MT_ZombieAttack.MT_ZombieAttack"));
	if (ATTACK_MONTAGE.Succeeded()) AttackMontage = ATTACK_MONTAGE.Object;
}

void UPA_Enemy_AnimInstance_Zombie::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
	IsAttack = true;
}