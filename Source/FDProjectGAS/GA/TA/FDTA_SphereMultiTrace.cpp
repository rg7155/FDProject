// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/FDTA_SphereMultiTrace.h"
#include "FDProjectGAS.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/FDCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"

FGameplayAbilityTargetDataHandle AFDTA_SphereMultiTrace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	const UFDCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UFDCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	//결과는 배열로 저장
	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = SkillAttribute->GetSkillRange();

	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AFDTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_FDACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}

	//TargetDataHandle에 들어갈 Data 선언
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	//인자는 약포인터의 배열로 받음
	ActorsData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}

#endif

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
