// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include <Runtime\AIModule\Classes\BehaviorTree\BlackboardComponent.h>
#include <Runtime\AIModule\Classes\Blueprint\AIBlueprintHelperLibrary.h>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 1.0;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::TakeOtherDamage(float Damage)
{
	Health -= Damage;
	UBlackboardComponent* MyBlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(GetOwner());
	if (MyBlackBoard)
	{
		MyBlackBoard->SetValueAsBool("BeAttack", true);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacter::SetBlackBoardBeAttackedState, 2.0, false);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Get BlackBoard Error"));

}

void AEnemyCharacter::SetBlackBoardBeAttackedState()
{
	UBlackboardComponent *MyBlackBoard = UAIBlueprintHelperLibrary::GetBlackboard(GetOwner());
	if (MyBlackBoard)
	{
		MyBlackBoard->SetValueAsBool("BeAttack", false);

	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Get BlackBoard Error"));

}

