// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TPSPlayer.h"
#include "Game/TPSProject.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Player/Bullet.h"
#include "AI/EnemyFSM.h"
#include "Action/PlayerAnim.h"
#include "Player/PlayerMove.h"
#include "Player/PlayerFire.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
		UE_LOG(LogTemp, Log, TEXT("Load"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Cant Load"));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	//Gun SK_Mesh Load
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(),TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TmpGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TmpGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TmpGunMesh.Object);
		
		gunMeshComp->SetRelativeLocation_Direct(FVector(-17, 10, -3));
		gunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
		UE_LOG(LogTemp, Log, TEXT("Gun Load"));
	}

	//Regist Snipergun component
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh(),TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TmpSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (TmpSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TmpSniperMesh.Object);

		//Position Control
		sniperGunComp->SetRelativeLocation(FVector(-42, 7, 1));
		sniperGunComp->SetRelativeRotation(FRotator(0, 90, 0));

		//Size Control
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
		UE_LOG(LogTemp, Log, TEXT("Sniper Gun Load"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Sniper Gun Cant Load"));
	}

	//IMC AssetLoad
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Player.IMC_Player'"));
	if (DEFAULT_CONTEXT.Succeeded())
		PlayerInputContext = DEFAULT_CONTEXT.Object;


	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	hp = initialHp;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(PlayerInputContext, 0);
	}

	

	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	onInputBindingDelegate.Broadcast(PlayerInputComponent);
}

void ATPSPlayer::OnHitEvent()
{
	PRINT_LOG(TEXT("Damaged!!!!"));
	hp--;
	if (hp <= 0)
	{
		PRINT_LOG(TEXT("Player is dead!!!!"));
	}
}












