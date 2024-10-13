// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APanCharacterPlayer::APanCharacterPlayer()
{
	// 컴포넌트 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// 종속 관계 설정
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// 스프링암 길이
	CameraBoom->TargetArmLength = 400.f;

	// 폰의 회전을 따를 지
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = false;
}
