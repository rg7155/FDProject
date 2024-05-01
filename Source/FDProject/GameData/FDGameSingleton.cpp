// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/FDGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UFDGameSingleton::UFDGameSingleton()
{
	//데이터 테이블 가져옴, 이는 키(Name), 밸류
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/FDProject/GameData/FDCharacterStatTable.FDCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		//밸류 값만 저장함
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
		{
			return *reinterpret_cast<FFDCharacterStat*>(Value);
		}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UFDGameSingleton& UFDGameSingleton::Get()
{
	//플젝 세팅해주면 GEngine 전역변수에 할당해줌
	UFDGameSingleton* Singleton = CastChecked<UFDGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	//임시 인스턴스 생성
	return *NewObject<UFDGameSingleton>();
}