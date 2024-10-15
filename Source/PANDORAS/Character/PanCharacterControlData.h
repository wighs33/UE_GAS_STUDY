// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PanCharacterControlData.generated.h"

/*************************************************************************************************
 * 시점에 따른 캐릭터 컨트롤 데이터
 *
 * @author	조현식
 * @date	2024/10/14
 **************************************************************************************************/
UCLASS()
class PANDORAS_API UPanCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPanCharacterControlData();

	// 캐릭터가 컨트롤러의 Yaw값 사용할 지 여부
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// 캐릭터가 이동하는 방향으로 자동회전 여부
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	// 캐릭터가 컨트롤러의 회전 방향 따를 지 여부
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	// 캐릭터 회전 속도
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	// 입력 매핑 컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// 스프링암 길이
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength = 400;

	// 스프링암 상대회전값
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	// 스프링암이 폰의 회전값 따를 지 여부
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	// 스프링암 Pitch, Yaw, Roll 상속여부
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	// 스프링암 충돌 검사 여부
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
