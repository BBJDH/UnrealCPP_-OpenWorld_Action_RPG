#pragma once
/*************************************
�ּ� ����
**************************************/

#include "DrawDebugHelpers.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"//SpawnEmitter

#include "Utilities/CHelpers.h"
#include "Utilities/CLog.h"



//TODO: (22-07-14) ���� �ؾ����� ǥ��
/*
 *�����޺� ����
 * - �׼� ��Ÿ�� �ִ� �޺� �ε��� ã�Ƽ� �ű������ ������� �Ұ�
 * - UpperAttack �� ActionName���� ã�Ƽ� ����� ��
 * - ActionName���� �����޺� ����� ��, InAir ���� ������ ���̺� �߰��Ұ�
 *
 */
//HACK: (22-07-14)  / ... �̰͵� ���������� 

//�̷� Ű���带 ���� �����ϴ� 
//���� - �ɼ� - �۾� ���