// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyRankingList.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTCOURSEPROJECT_API UMyRankingList : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ranking")
		TArray<int32> InsertScore(TArray<int32> scores, int32 score);
};
