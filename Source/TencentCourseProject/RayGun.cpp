// Fill out your copyright notice in the Description page of Project Settings.


#include "RayGun.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FPSCharacter.h"
#include "Engine/World.h"
#include "Target.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARayGun::ARayGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BIsOnCharacter = false;
	OwnedCharacter = nullptr;
}

// Called when the game starts or when spawned
void ARayGun::BeginPlay()
{
	Super::BeginPlay();

	UBoxComponent *Capsule = Cast<UBoxComponent>(RootComponent);
	if (Capsule)
	{
		//Capsule->OnComponentHit.AddDynamic(this, &ARayGun::OnHit);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ARayGun::OnCollision);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("UBoxComponent Cast Failed!"));

}

// Called every frame
void ARayGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARayGun::OnCollision(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AFPSCharacter::StaticClass()))
	{
		if (!BIsOnCharacter)
		{
			AFPSCharacter *Character = Cast<AFPSCharacter>(OtherActor);
			if (Character)
			{
				OwnedCharacter = Character;
				Character->PickupGun(this);
			}
		}
	}
}

void ARayGun::Fire(const FVector &Pos, const FVector &Dir)
{
	UWorld *World = GetWorld();
	FVector P = Pos;
	UParticleSystemComponent *FirePartical = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));
	if (FirePartical)
		FirePartical->ActivateSystem();
	FHitResult OutHit;
	float LineDis = 10000;
	FVector End = Pos + Dir * LineDis;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Pos, End, ECC_Visibility))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
		
		// Hit Character  SK_Mannequin
		if (OutHit.GetActor()->GetName() == FString("SK_Mannequin_5"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting bone: %s"), *OutHit.BoneName.ToString()));
			if(OutHit.BoneName == FName("head"))
				UGameplayStatics::PlaySoundAtLocation(World, SoundHitHead, OutHit.ImpactPoint);
			else if (OutHit.BoneName == FName("spine_01") || OutHit.BoneName == FName("spine_02"))
				UGameplayStatics::PlaySoundAtLocation(World, SoundHitBody, OutHit.ImpactPoint);
			else
				UGameplayStatics::PlaySoundAtLocation(World, SoundHitElse, OutHit.ImpactPoint);

		}

		// Hit Target
		if (OutHit.GetActor()->IsA(ATarget::StaticClass()))
		{
			// Compute distance from target center
			ATarget *target = Cast<ATarget>(OutHit.GetActor());
			FVector TargetPos, TargetBox;
			target->GetActorBounds(false, TargetPos, TargetBox);
			FVector HitPos = OutHit.ImpactPoint;
			float Distance = FVector::Dist(TargetPos, HitPos);
			float Ration = Distance / TargetBox.Z;
			// 0.27 0.44 0.65 0.83
			// compute score
			int32 score = 0;
			if (Ration < 0.27)
				score = 10;
			else if (Ration < 0.44)
				score = 6;
			else if (Ration < 0.65)
				score = 4;
			else if (Ration < 0.83)
				score = 2;

			if(OwnedCharacter)
				OwnedCharacter->GetScore(score);
			GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("score: " + FString::FromInt(score)));

			target->AddBulletHole(HitPos); // bullet hole decal
		}
	}
}
