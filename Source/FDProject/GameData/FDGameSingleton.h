// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FDCharacterStat.h"
#include "FDGameSingleton.generated.h"

//�̱��� ���� �� ����
DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UFDGameSingleton();
	static UFDGameSingleton& Get();

	// Character Stat Data Section
public:
	FORCEINLINE FFDCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FFDCharacterStat(); }

	UPROPERTY()
	//�� ��� ������ �ִ���
	int32 CharacterMaxLevel;

private:
	TArray<FFDCharacterStat> CharacterStatTable;
};
