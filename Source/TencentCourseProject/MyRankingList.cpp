// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRankingList.h"

TArray<int32> UMyRankingList::InsertScore(TArray<int32> scores, int32 score)
{
	if (scores.Last() < score)
	{
		scores[scores.Num() - 1] = score;
		scores.Sort([](const int32& s1, const int32& s2) {
			return s1 > s2;
			});
	}
	return scores;
}