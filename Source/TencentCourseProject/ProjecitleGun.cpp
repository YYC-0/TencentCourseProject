// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjecitleGun.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FPSCharacter.h"

// Sets default values
AProjecitleGun::AProjecitleGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BIsOnCharacter = false;
}

// Called when the game starts or when spawned
void AProjecitleGun::BeginPlay()
{
	Super::BeginPlay();

	UBoxComponent *Capsule = Cast<UBoxComponent>(RootComponent);
	if (Capsule)
	{
		//Capsule->OnComponentHit.AddDynamic(this, &ARayGun::OnHit);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &AProjecitleGun::OnCollision);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("UBoxComponent Cast Failed!"));

}

// Called every frame
void AProjecitleGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjecitleGun::OnCollision(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AFPSCharacter::StaticClass()))
	{
		if (!BIsOnCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Hit"));
			AFPSCharacter *Character = Cast<AFPSCharacter>(OtherActor);
			if (Character)
			{
				Character->PickupGun(this);
				BIsOnCharacter = true;
			}
		}
	}
}

void AProjecitleGun::Fire(const FVector & Pos, const FVector & Dir)
{
	UWorld *World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AFPSProjectile *Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, Pos, GetActorRotation(), SpawnParams);
		if (Projectile)
		{
			Projectile->FireInDirection(Dir);
		}
	}
}
