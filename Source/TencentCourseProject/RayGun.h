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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	
	// pickup gun
	UFUNCTION()
		void OnCollision(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Fire
	UFUNCTION(Server, Reliable)
		void Fire(const FVector& Pos, const FVector& Dir);

	void SetOwnedCharacter(AFPSCharacter *Character) { OwnedCharacter = Character; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundBase* SoundHitHead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundBase* SoundHitBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
		USoundBase* SoundHitElse;

	void PlaySound(FName HitBone, FVector PlayPos);
protected:
	UFUNCTION()
		void OnRep_IsFireTest();
	UFUNCTION()
		void OnRep_HitCharacter();

	UPROPERTY(ReplicatedUsing = OnRep_IsFireTest)
		bool IsFire;
		FName HitBone;
		FVector HitPos;
private:
	bool BIsOnCharacter;
	AFPSCharacter *OwnedCharacter;

};
