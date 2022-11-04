// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


	/*************************************
	__FILE__ : ��ũ��, ���ϸ�
	__FUNCTION__ : ���� �Ҽӵ� �Լ�,
	__LINE__ : ���� �ִ� ����

	**************************************/
#define LogLine() {CLog::Log(__FILE__, __FUNCTION__, __LINE__);}
#define PrintLine() {CLog::Print(__FILE__, __FUNCTION__, __LINE__);}


class TPS_RPG_API CLog
{
public:
	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(FString const & InValue);
	static void Log(FVector const & InValue);
	static void Log(FRotator const & InValue);
	static void Log(UObject const * InValue);
	static void Log(FString const & InValue, FString const & InFuncName, int32 InLineNumber);

	//Key:0 �̻��̸� ��ġ ����, Duration: ���ʵ��� ��������, COlor: ���ڻ�
	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(FString const & InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(FVector const & InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(FRotator const & InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(UObject const * InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(FString const & InFileName, FString const & InFuncName, int32 InLineNumber);
};
