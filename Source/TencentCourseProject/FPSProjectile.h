// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AFPSCharacter;

UCLASS()
class TENCENTCOURSEPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// setting projectile direction and speed
	void FireInDirection(const FVector& ShootDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
		float ShootForce;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent *CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent *ProjectileMoveComponent;

	// ��������������ʱ����
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float FireSpeed;

private:
	bool hitted;
};
