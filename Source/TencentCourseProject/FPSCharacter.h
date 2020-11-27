// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSProjectile.h"
#include "FPSCharacter.generated.h"

class ARayGun;
class AProjecitleGun;

UCLASS()
class TENCENTCOURSEPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// pickup gun
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Fire
	UFUNCTION(BlueprintCallable, Category = "CharacterAction")
	void Fire();

	void GetScore(int32 s);
	void ClearScore();

	void PickupGun(ARayGun* RayGun);
	void PickupGun(AProjecitleGun* ProjectileGun);
	void DropGun();

	// �������λ�õ�ǹ��ƫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	// ���ɷ�������
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	// score
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float EnergyReduceSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float EnergyRestoreSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 AmmoMax;

private:
	enum GunType
	{
		Ray,
		Projectile
	};
	GunType CurrentGunType;

	ARayGun *RayGun;
	AProjecitleGun *ProjectileGun;
	TSubclassOf<AProjecitleGun> ProjecitleGunBP;
	TSubclassOf<ARayGun> RayGunBP;
};
