// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "RayGun.h"
#include "ProjecitleGun.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Score = 0;
	Health = 1.0;
	Energy = 0.5;
	AmmoMax = 10;
	Ammo = AmmoMax;
	RayGun = nullptr;
	ProjectileGun = nullptr;

	//static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileGun_BP(TEXT("Blueprint'/Game/Blueprints/BP_ProjecitleGun.BP_ProjecitleGun'"));
	//if (ProjectileGun_BP.Object)
	//	ProjecitleGunBP = (UClass*)ProjectileGun_BP.Object->GeneratedClass;
	//else
	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Construct Projectile Gun BP ERROR"));

	//static ConstructorHelpers::FObjectFinder<UBlueprint> RayGun_BP(TEXT("Blueprint'/Game/Blueprints/BP_RayGun.BP_RayGun'"));
	//if (RayGun_BP.Object)
	//	RayGunBP = (UClass*)RayGun_BP.Object->GeneratedClass;
	//else
	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Construct Ray Gun BP ERROR"));
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent *Capsule = Cast<UCapsuleComponent>(RootComponent);
	if (Capsule)
	{
		Capsule->OnComponentHit.AddDynamic(this, &AFPSCharacter::OnHit);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Capsule Cast Error!"));

	if (RayGunBP)
	{
		ARayGun *Gun = GetWorld()->SpawnActor<ARayGun>(RayGunBP);
		PickupGun(Gun);
		Gun->SetOwnedCharacter(this);
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

void AFPSCharacter::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{

}

void AFPSCharacter::Fire()
{
	if (Ammo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Need reload ammo!"));
		return;
	}
	Ammo -= 1;

	//if (ShootingAudio)
	//{
	//	ShootingAudio->Play();
	//}
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);
	APlayerCameraManager *CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	FVector FireLocation;
	FRotator FireRotation;
	FireLocation = CurrentCamera->GetCameraLocation();
	FireRotation = CurrentCamera->GetCameraRotation();
	FireRotation.Pitch += 6.5f;
	FireLocation = FireLocation + 250 * FireRotation.Vector();

	if (RayGun)
		RayGun->Fire(FireLocation, FireRotation.Vector());
	else if (ProjectileGun)
		ProjectileGun->Fire(FireLocation, FireRotation.Vector());
	//if (ProjectileClass)
	//{
	//	UWorld *World = GetWorld();
	//	if (World)
	//	{
	//		FActorSpawnParameters SpawnParams;
	//		SpawnParams.Owner = this;
	//		SpawnParams.Instigator = GetInstigator();

	//		// ��ǹ�ڴ����ɷ�����
	//		AFPSProjectile *Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, FireLocation, FireRotation, SpawnParams);
	//		if (Projectile)
	//		{
	//			// ���ó�ʼ�켣
	//			FVector LaunchDirection = FireRotation.Vector();
	//			Projectile->FireInDirection(this, LaunchDirection);
	//		}
	//	}
	//}
}

void AFPSCharacter::GetScore(int32 s)
{
	Score += s;
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::FromInt(score));
	if (Score >= 100) // win
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("YOU WIN!"));
	}
}

void AFPSCharacter::ClearScore()
{
	Score = 0;
}

void AFPSCharacter::PickupGun(ARayGun *NewRayGun)
{
	DropGun();
	RayGun = NewRayGun;
	NewRayGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("WeaponSocket")));
	CurrentGunType = GunType::Ray;
}

void AFPSCharacter::PickupGun(AProjecitleGun *NewProjectileGun)
{
	DropGun();
	ProjectileGun = NewProjectileGun;
	NewProjectileGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("WeaponSocket")));
	CurrentGunType = GunType::Projectile;
}

void AFPSCharacter::DropGun()
{
	if (ProjectileGun)
	{
		ProjectileGun->Destroy();
		ProjectileGun = nullptr;
		FVector Location = GetActorLocation() + GetActorForwardVector() * 150;
		Location.Z = 170;

		GetWorld()->SpawnActor<AProjecitleGun>(ProjecitleGunBP, Location, FRotator());
	}
	else if (RayGun)
	{
		RayGun->Destroy();
		RayGun = nullptr;

		FVector Location = GetActorLocation() + GetActorForwardVector() * 150;
		Location.Z = 170;
		GetWorld()->SpawnActor<ARayGun>(RayGunBP, Location, FRotator());
	}
}
