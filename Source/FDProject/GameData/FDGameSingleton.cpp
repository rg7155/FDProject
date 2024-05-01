// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/FDGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UFDGameSingleton::UFDGameSingleton()
{
	//������ ���̺� ������, �̴� Ű(Name), ���
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/FDProject/GameData/FDCharacterStatTable.FDCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		//��� ���� ������
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
	//���� �������ָ� GEngine ���������� �Ҵ�����
	UFDGameSingleton* Singleton = CastChecked<UFDGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	//�ӽ� �ν��Ͻ� ����
	return *NewObject<UFDGameSingleton>();
}