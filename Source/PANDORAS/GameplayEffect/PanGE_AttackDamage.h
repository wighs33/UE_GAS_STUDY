 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayEffect\PanGE_AttackDamage.h
 *
 * #include "GameplayEffect/PanGE_AttackDamage.h"
 * 데미지 관리 객체 (BPGE_AttackDamage 블루프린트로 대체)
 * 
 * 아래 블루프린트에 의해 (콤보데미지 X 공격력 = 데미지)로 계산
 * <BPGE_AttackDamage>
 * 모디파이어
 *  - 어트리뷰트 : PanCharacterAttributeSet.Damage
 *  - 연산자 : 더하기
 *  - 수치
 *	 - 계산 방식 : 어트리뷰트 이용
 *	  - 이용할 어트리뷰트 수치
 *	   - 계수 : 1.0, ComboDamageTable의 ComboDamage
 *	   - 이용할 어트리뷰트 : PanCharacterAttributeSet.AttackRate
 *	   - 어트리뷰트 곡선 : 어트리뷰트 기본값
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PanGE_AttackDamage.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanGE_AttackDamage : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UPanGE_AttackDamage();
};
