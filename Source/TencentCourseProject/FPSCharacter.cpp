// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"

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
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent *Audio = GetComponentByClass(UAudioComponent::StaticClass());
	if (Audio)
	{
		ShootingAudio = Cast<UAudioComponent>(Audio);
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

	if (ProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		APlayerCameraManager *CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

		// �� MuzzleOffset ��������ռ�任������ռ�
		//FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		//FRotator MuzzleRotation = CameraRotation;

		FVector MuzzleLocation;
		FRotator MuzzleRotation;
		MuzzleLocation = CurrentCamera->GetCameraLocation();
		MuzzleRotation = CurrentCamera->GetCameraRotation();
		MuzzleLocation = MuzzleLocation + MuzzleRotation.Vector() * 50;
		
		MuzzleRotation.Pitch += 8.0f;
		UWorld *World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// ��ǹ�ڴ����ɷ�����
			AFPSProjectile *Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// ���ó�ʼ�켣
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(this, LaunchDirection);
			}
		}
	}
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