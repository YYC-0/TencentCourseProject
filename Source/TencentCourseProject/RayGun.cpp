// Fill out your copyright notice in the Description page of Project Settings.


#include "RayGun.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FPSCharacter.h"
#include "Engine/World.h"
#include "Target.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnemyCharacter.h"

// Sets default values
ARayGun::ARayGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BIsOnCharacter = false;
	OwnedCharacter = nullptr;
	IsFire = false;
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

void ARayGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARayGun, IsFire);
	//DOREPLIFETIME(ARayGun, HitBone);
	//DOREPLIFETIME(ARayGun, HitPos);
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

void ARayGun::PlaySound(FName HitBone_, FVector PlayPos_)
{
	UWorld *World = GetWorld();
	if (HitBone_ == FName("head"))
		UGameplayStatics::PlaySoundAtLocation(World, SoundHitHead, PlayPos_);
	else if (HitBone_ == FName("spine_01") || HitBone == FName("spine_02"))
		UGameplayStatics::PlaySoundAtLocation(World, SoundHitBody, PlayPos_);
	else
		UGameplayStatics::PlaySoundAtLocation(World, SoundHitElse, PlayPos_);
}

void ARayGun::OnRep_IsFireTest()
{
	if (IsFire)
	{
		UParticleSystemComponent *FirePartical = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));
		if (FirePartical)
			FirePartical->ActivateSystem();
	}
}

void ARayGun::OnRep_HitCharacter()
{
	//PlaySound(HitBone, HitPos);
}

void ARayGun::Fire_Implementation(const FVector &Pos, const FVector &Dir)
{
	IsFire =  true;
	UWorld *World = GetWorld();
	if(!World)
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Get World Failed"));

	FVector P = Pos;
	UParticleSystemComponent *FirePartical = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));
	if (FirePartical)
		FirePartical->ActivateSystem();
	FHitResult OutHit;
	float LineDis = 10000;
	FVector End = Pos + Dir * LineDis;
	if (World->LineTraceSingleByChannel(OutHit, Pos, End, ECC_Visibility))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));

		// Hit Character  SK_Mannequin
		if (OutHit.GetActor()->GetName() == FString("SK_Mannequin_5"))
		{
			PlaySound(OutHit.BoneName, OutHit.ImpactPoint);
			HitBone = OutHit.BoneName;
			HitPos = OutHit.ImpactPoint;
		}
		// Hit Character
		if (OutHit.GetActor()->IsA(AFPSCharacter::StaticClass()))
		{
			//PlaySound(OutHit.BoneName, OutHit.ImpactPoint);
			HitBone = OutHit.BoneName;
			HitPos = OutHit.ImpactPoint;
			AFPSCharacter *Character = Cast<AFPSCharacter>(OutHit.GetActor());
			if (Character)
			{
				Character->TakeOtherDamage(0.26f, OwnedCharacter);
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Cast character failed!"));
		}

		// Hit Target
		if (OutHit.GetActor()->IsA(ATarget::StaticClass()))
		{
			// Compute distance from target center
			ATarget *target = Cast<ATarget>(OutHit.GetActor());
			FVector TargetPos, TargetBox;
			target->GetActorBounds(false, TargetPos, TargetBox);
			FVector ImpactPos = OutHit.ImpactPoint;
			float Distance = FVector::Dist(TargetPos, ImpactPos);
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
			//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("score: " + FString::FromInt(score)));

			target->AddBulletHole(ImpactPos); // bullet hole decal
		}

		// Hit Enemy
		if(OutHit.GetActor()->IsA(AEnemyCharacter::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Hit enemy!"));

			AEnemyCharacter *Enemy = Cast<AEnemyCharacter>(OutHit.GetActor());
			if (Enemy)
				Enemy->TakeOtherDamage(0.1);
			else
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Cast enemy character failed!"));
		}

	}
	IsFire = false;
}
