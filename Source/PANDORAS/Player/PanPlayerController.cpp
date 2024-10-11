// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PanPlayerController.h"

void APanPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임에 입력을 바로 전달하여 게임 시작 시 뷰포트 포커싱
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	// 입력 모드 종류
	// FInputModeGameOnly : 게임에만 입력 전달
	// FInputModeGameOnly : UI에만 입력 전달
	// FInputModeGameAndUI : 둘 다 전달
}
