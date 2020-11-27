// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayGun.generated.h"
class AFPSCharacter;

UCLASS()
class TENCENTCOURSEPROJECT_API ARayGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARayGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// pickup gun
	UFUNCTION()
		void OnCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Fire
	UFUNCTION()
		void Fire(const FVector& Pos, const FVector& Dir);

	void SetOwnedCharacter(AFPSCharacter *Character) { OwnedCharacter = Character; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundWave* SoundHitHead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundWave* SoundHitBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundWave* SoundHitElse; 

private:
	bool BIsOnCharacter;
	AFPSCharacter *OwnedCharacter;
};
