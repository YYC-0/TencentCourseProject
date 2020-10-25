// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Components\DecalComponent.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATarget::AddBulletHole(FVector Position)
{
	// Decal
	UMaterial *DecalMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/Materials/M_Bullet_Hole"));
	UDecalComponent *DecalComponent = NewObject<UDecalComponent>(RootComponent);
	if (DecalComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Decal Success"));
		DecalComponent->RegisterComponent();
		DecalComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		DecalComponent->SetDecalMaterial(DecalMaterial);
		DecalComponent->DecalSize = FVector(140.0f, 2.0f, 2.0f);
		DecalComponent->SetWorldLocation(Position);
	}
	//else
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Decal Fail"));
}

