// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjecitleGun.generated.h"

UCLASS()
class TENCENTCOURSEPROJECT_API AProjecitleGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjecitleGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// pickup gun
	UFUNCTION()
		void OnCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Fire
	UFUNCTION()
		void Fire(const FVector& Pos, const FVector& Dir);
private:
	bool BIsOnCharacter;

};
