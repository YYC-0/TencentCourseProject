// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSProjectile.h"
#include "FPSCharacter.generated.h"

class ARayGun;
class AProjecitleGun;
class AMyPlayerState;

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
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "CharacterAction")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "CharacterAction")
		void KillCharacter(AFPSCharacter *Character);

	void GetScore(int32 s);
	void ClearScore();

	void PickupGun(ARayGun* RayGun);
	void PickupGun(AProjecitleGun* ProjectileGun);
	void DropGun();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
		void TakeOtherDamage(float Damage, AFPSCharacter *TakeDamageCharacter);

	UFUNCTION()
		void ResetState();

protected:
	void OnHealthUpdate();
	
	UFUNCTION()
		void OnRep_CurrentHealth();
	UFUNCTION()
		void OnRep_IsDead();
	UFUNCTION()
		void OnRep_IsFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float EnergyReduceSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float EnergyRestoreSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Gameplay)
		int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 AmmoMax;
	// score
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int32 Score;
	// kill number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		int32 KillNum;
	// dead number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, Replicated)
		int32 DeadNum;
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
		bool IsDead;
	UPROPERTY(ReplicatedUsing = OnRep_IsFire)
		bool IsFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BPClasses)
		TSubclassOf<AProjecitleGun> ProjecitleGunBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BPClasses)
		TSubclassOf<ARayGun> RayGunBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
		UAnimMontage *DeadMontage;

	// �������λ�õ�ǹ��ƫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	// ���ɷ�������
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;
private:
	enum GunType
	{
		Ray,
		Projectile
	};
	GunType CurrentGunType;

	ARayGun *RayGun;
	AProjecitleGun *ProjectileGun; 

	FVector StartPos;

	FTimerHandle TimerHandle;

	UFUNCTION()
		void Dead();
	UFUNCTION()
		void Resurrection();

	//TSubclassOf<AProjecitleGun> ProjecitleGunBP;
	//TSubclassOf<ARayGun> RayGunBP;
};
