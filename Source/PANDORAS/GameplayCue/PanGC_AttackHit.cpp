// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/PanGC_AttackHit.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UPanGC_AttackHit::UPanGC_AttackHit()
{
	// 파티클시스템 애셋 로드
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Script/Engine.ParticleSystem'/Game/Pandoras/Effect/P_Explosion.P_Explosion'"));
	// 오브젝트가 유효하다면
	if (ExplosionRef.Object)
	{
		// TObjectPtr 형식으로 저장
		ParticleSystem = ExplosionRef.Object;
	}
}

/*************************************************************************************************
 * 특정 큐를 실행했을 때 호출
 *
 * @author	조현식
 * @date	2024/12/16
 * @param	
 * @return	
 **************************************************************************************************/
bool UPanGC_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	// 충돌 결과 가져오는 데 성공하면
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		// 충돌 결과의 ImpactPoint로부터 파티클 이펙트를 스폰
		UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else
	{
		// 다수의 액터 정보가 들어온다면 타겟 액터마다
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
			{
				// 충돌 결과의 ImpactPoint로부터 파티클 이펙트를 스폰
				UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}

	return false;
}