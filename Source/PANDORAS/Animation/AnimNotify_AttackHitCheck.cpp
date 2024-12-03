// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_AttackHitCheck::UAnimNotify_AttackHitCheck()
{
}

FString UAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("AttackHitCheck");
}

/*************************************************************************************************
 * 노티파이에 걸렸을 때 대상에게 공격이 맞았는 지 판정
 *
 * @author	조현식
 * @date	2024/11/27
 * @param	스켈레탈 메시, 애니메이션, 이벤트
 **************************************************************************************************/
void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	// 메시 존재
	if (MeshComp)
	{
		// 스켈레탈 메시로부터 캐릭터 얻기
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			// 임시 이벤트
			FGameplayEventData PayloadData;
			// 액터에 태그를 넣어서 이벤트를 발동시킨다
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggetGameplayTag, PayloadData);
		}



		//// 스켈레탈 메시로부터 캐릭터 얻기
		//IPanAnimationAttackInterface* AttackPawn = Cast<IPanAnimationAttackInterface>(MeshComp->GetOwner());
		//if (AttackPawn)
		//{
		//	// 캐릭터의 공격 판정
		//	AttackPawn->AttackHitCheck();
		//}
	}
}
