#pragma once
/*************************************
주석 내용
**************************************/

#include "DrawDebugHelpers.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"//SpawnEmitter

#include "Utilities/CHelpers.h"
#include "Utilities/CLog.h"



//TODO: (22-07-14) 오늘 해야할일 표시
/*
 *공중콤보 구현
 * - 액션 몽타주 최대 콤보 인덱스 찾아서 거기까지만 연결토록 할것
 * - UpperAttack 시 ActionName으로 찾아서 재생할 것
 * - ActionName으로 공종콤보 재생될 것, InAir 유무 데이터 테이블에 추가할것
 *
 */
//HACK: (22-07-14)  / ... 이것도 마찬가지로 

//이런 키워드를 생성 가능하다 
//도구 - 옵션 - 작업 목록