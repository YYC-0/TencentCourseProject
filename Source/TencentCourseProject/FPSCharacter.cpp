// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "RayGun.h"
#include "ProjecitleGun.h"
#include "Net/UnrealNetwork.h"
#include "MyPlayerState.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 1.0f;
	AmmoMax = 20;
	Score = 0;
	KillNum = 0;
	DeadNum = 0;
	IsDead = false;
	RayGun = nullptr;
	ProjectileGun = nullptr;
	IsFire = false;
	ResetState();

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
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Capsule Cast Error!"));

	if (RayGunBP)
	{
		ARayGun *Gun = GetWorld()->SpawnActor<ARayGun>(RayGunBP);
		PickupGun(Gun);
		Gun->SetOwnedCharacter(this);
	}

	StartPos = GetActorLocation();

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

void AFPSCharacter::Fire_Implementation()
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
	//FVector CameraLocation;
	//FRotator CameraRotation;
	//GetActorEyesViewPoint(CameraLocation, CameraRotation);
	//APlayerCameraManager *CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	UCameraComponent *CameraComps = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
	FVector FireLocation;
	FRotator FireRotation;
	if (CameraComps)
	{
		FireLocation = CameraComps->GetComponentLocation();
		FireRotation = CameraComps->GetComponentRotation();
		FireRotation.Pitch += 6.5f;
		FireLocation = FireLocation + 250 * FireRotation.Vector();
	}
	//FireLocation = CurrentCamera->GetCameraLocation();
	//FireRotation = CurrentCamera->GetCameraRotation();
	//FireRotation.Pitch += 6.5f;
	//FireLocation = FireLocation + 250 * FireRotation.Vector();


	if (RayGun)
		RayGun->Fire(FireLocation, FireRotation.Vector());
	else if (ProjectileGun)
		ProjectileGun->Fire(FireLocation, FireRotation.Vector());
}

void AFPSCharacter::KillCharacter(AFPSCharacter *Character)
{
	KillNum++;
	AMyPlayerState *MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if (MyPlayerState)
		MyPlayerState->SetKillNum(KillNum);
	else
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Player State Cast Error!"));

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
	UBoxComponent *box = Cast<UBoxComponent>(RayGun->GetComponentByClass(UBoxComponent::StaticClass()));
	if (box)
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Get box component failed"));
	NewRayGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("WeaponSocket")));
	CurrentGunType = GunType::Ray;
}

void AFPSCharacter::PickupGun(AProjecitleGun *NewProjectileGun)
{
	DropGun();
	ProjectileGun = NewProjectileGun;
	UBoxComponent *box = Cast<UBoxComponent>(ProjectileGun->GetComponentByClass(UBoxComponent::StaticClass()));
	if (box)
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Get box component failed"));
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

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//复制当前生命值。
	DOREPLIFETIME(AFPSCharacter, CurrentHealth);

	DOREPLIFETIME(AFPSCharacter, DeadNum);
	DOREPLIFETIME(AFPSCharacter, KillNum);
	DOREPLIFETIME(AFPSCharacter, IsDead);
	DOREPLIFETIME(AFPSCharacter, IsFire);
	DOREPLIFETIME(AFPSCharacter, Ammo);
}

void AFPSCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AFPSCharacter::OnRep_IsDead()
{
	if (IsDead)
	{
		float deadTime = 1.0f;
		if (DeadMontage)
			deadTime = PlayAnimMontage(DeadMontage) - 0.1;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFPSCharacter::Resurrection, deadTime, false);
	}
}

void AFPSCharacter::OnRep_IsFire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("AFPSCharacter::OnRep_IsFire"));
}

void AFPSCharacter::Dead()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You are dead!"));
	IsDead = true;
	DeadNum++;

	if (GetLocalRole() == ROLE_Authority)
	{
		float deadTime = 1.0f;
		if (DeadMontage)
			deadTime = PlayAnimMontage(DeadMontage) - 0.1;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFPSCharacter::Resurrection, deadTime, false);
	}

	AMyPlayerState *MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if(MyPlayerState)
		MyPlayerState->SetDeadNum(DeadNum);
	else
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Player State Cast Error!"));

}

void AFPSCharacter::Resurrection()
{
	// 复活
	ResetState();
	SetActorLocation(StartPos + FVector(0, 0, 200));
	SetActorRotation(FRotator::ZeroRotator);
}

void AFPSCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.0f, MaxHealth);
		OnHealthUpdate();
	}
}

void AFPSCharacter::TakeOtherDamage(float Damage, AFPSCharacter *TakeDamageCharacter)
{
	if (!IsDead)
	{
		float HealthRemain = CurrentHealth - Damage;
		if (GetLocalRole() == ROLE_Authority)
			if (HealthRemain <= 0.f)
			{
				Dead();
				TakeDamageCharacter->KillCharacter(this);
			}
		SetCurrentHealth(HealthRemain);
	}
}

void AFPSCharacter::ResetState()
{
	CurrentHealth = MaxHealth;
	Energy = 1.0;
	Ammo = AmmoMax;
	IsDead = false;
}

void AFPSCharacter::OnHealthUpdate()
{
	////客户端特定的功能
	//if (IsLocallyControlled())
	//{
	//	FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

	//	if (CurrentHealth <= 0)
	//	{
	//		FString deathMessage = FString::Printf(TEXT("You have been killed."));
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
	//	}
	//}

	////服务器特定的功能
	//if (GetLocalRole() == ROLE_Authority)
	//{
	//	FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	//}
}
