// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/PanGameMode.h"

APanGameMode::APanGameMode()
{
	// 디폴트 폰 클래스는 플레이어 블루프린트
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Pandoras/Character/BP_PanCharacterPlayer.BP_PanCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	// 디폴트 컨트롤러 클래스는 컨트롤러 스크립트
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/PANDORAS.PanPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
