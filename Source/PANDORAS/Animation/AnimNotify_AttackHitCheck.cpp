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
			// 페이로드 데이터 (전송 데이터에서 부가 데이터를 제외한 목적 데이터)
			FGameplayEventData PayloadData;
			// 콤보 공격 횟수를 값으로 설정
			PayloadData.EventMagnitude = ComboAttackLevel;
			// 액터에 태그를 넣어서 이벤트를 발동시킨다
			// 어빌리티의 기본 함수인 ActivateAbility의 TriggerEventData로 데이터가 전송됨
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
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
