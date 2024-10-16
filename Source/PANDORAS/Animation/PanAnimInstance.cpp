// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PanAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPanAnimInstance::UPanAnimInstance()
{
}

/*************************************************************************************************
 * 애님 인스턴스가 처음 생성될 때 호출
 * 소유자 파악
 *
 * @author	조현식
 * @date	2024/10/16
 **************************************************************************************************/
void UPanAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 소유자 얻기
	if (Owner = Cast<ACharacter>(GetOwningActor()))
	{
		// 소유자를 통해 움직임 얻기
		Movement = Owner->GetCharacterMovement();
	}
}

/*************************************************************************************************
 * 정해진 프레임마다 호출
 * 캐릭터의 여러 상태 파악
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	초
 **************************************************************************************************/
void UPanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		// 속도 얻기
		Velocity = Movement->Velocity;
		// 지상속도
		GroundSpeed = Velocity.Size2D();
		// 지상속도가 이동 임계점 못 넘으면 '대기 상태'
		bIsIdle = GroundSpeed < MovingThreshould;
		// '추락 상태'는 무브먼트로 획득가능
		bIsFalling = Movement->IsFalling();
		// 추락 상태인 동시에 점프 임계점 초과 시 '점프 상태'
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
