// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTCOURSEPROJECT_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void SetDeadNum(int32 Num);
	UFUNCTION(BlueprintCallable, Server, Reliable)
		void SetKillNum(int32 Num);

	UFUNCTION(BlueprintCallable)
	int32 GetKillNum() const { return KillNum; }
	UFUNCTION(BlueprintCallable)
	int32 GetDeadNum() const { return DeadNum; }

private:
	UPROPERTY(Replicated)
	int32 DeadNum;
	UPROPERTY(Replicated)
	int32 KillNum;
};
