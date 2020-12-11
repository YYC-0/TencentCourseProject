// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{
	DeadNum = 0;
	KillNum = 0;
}

void AMyPlayerState::SetDeadNum_Implementation(int32 Num)
{ 
	DeadNum = Num;
}

void AMyPlayerState::SetKillNum_Implementation(int32 Num)
{
	KillNum = Num;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, KillNum);
	DOREPLIFETIME(AMyPlayerState, DeadNum);
}
