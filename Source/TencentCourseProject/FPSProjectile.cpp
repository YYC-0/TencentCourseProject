// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "Components\SphereComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "Target.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile")); // ������ײԤ��
	CollisionComponent->InitSphereRadius(15.0f); // ��ײ�뾶
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	RootComponent = CollisionComponent; // ����ײ�����Ϊ�����

	ProjectileMoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMoveComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMoveComponent->InitialSpeed = 5000.0f;
	ProjectileMoveComponent->MaxSpeed = 5000.0f;
	ProjectileMoveComponent->bRotationFollowsVelocity = true;
	ProjectileMoveComponent->bShouldBounce = true;
	ProjectileMoveComponent->Bounciness = 0.3f;

	InitialLifeSpan = 3.0f; // ��������������

	hitted = false;
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSProjectile::FireInDirection(AFPSCharacter *Projector_, const FVector & ShootDirection)
{
	this->Projector = Projector_;
	ProjectileMoveComponent->Velocity = ShootDirection * ProjectileMoveComponent->InitialSpeed;
}

void AFPSProjectile::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
	if (!hitted && OtherActor->IsA(ATarget::StaticClass()))
	{
		// Hit Target
		//GEngine->AddOnScreenDebugMessage(0, 1.0, FColor::Red, TEXT("Hit Target!"));
		ProjectileMoveComponent->SetActive(false); // stop moving
		CollisionComponent->SetActive(false);
		hitted = true;

		// Compute distance from target center
		ATarget *target = Cast<ATarget>(OtherActor);
		FVector TargetPos, TargetBox;
		target->GetActorBounds(false, TargetPos, TargetBox);
		FVector ProjectilePos = GetActorLocation();
		float Distance = FVector::Dist(TargetPos, ProjectilePos);
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

		Projector->GetScore(score);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("score: " + FString::FromInt(score)));

		target->AddBulletHole(ProjectilePos); // bullet hole decal
	}

	this->Destroy();
}
