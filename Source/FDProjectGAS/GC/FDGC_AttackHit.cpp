// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/FDGC_AttackHit.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UFDGC_AttackHit::UFDGC_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_Impact_Enemy_Strong.P_Impact_Enemy_Strong'"));
	if (ExplosionRef.Object)
	{
		ParticleSystem = ExplosionRef.Object;
	}
}

//인자로 들어오는 Param에 여러 값이 들어오고 있음
bool UFDGC_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		//블프 함수
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else
	{
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
			{
				UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}
	return false;
}
