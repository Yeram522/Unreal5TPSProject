// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Bullet.h"
#include "EnemyFSM.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/CosmoBunnyGirl/Mesh/SK_CosmoBunny_with_bones_in_dress.SK_CosmoBunny_with_bones_in_dress'"));
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

	//Gun SK_Mesh Load
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TmpGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TmpGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TmpGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
		UE_LOG(LogTemp, Log, TEXT("Gun Load"));
	}

	//Regist Snioergun component
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TmpSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (TmpSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TmpSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
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

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVEFORWARD
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveForward.IA_MoveForward'"));
	if (IA_MOVEFORWARD.Succeeded())
		MoveForwardAction = IA_MOVEFORWARD.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVERIGHT
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveRight.IA_MoveRight'"));
	if (IA_MOVERIGHT.Succeeded())
		MoveRightAction = IA_MOVERIGHT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOKUp
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_LookUp.IA_LookUp'"));
	if (IA_LOOKUp.Succeeded())
		LookUpAction = IA_LOOKUp.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Turn
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Turn.IA_Turn'"));
	if (IA_Turn.Succeeded())
		TurnAction = IA_Turn.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Jump
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (IA_Jump.Succeeded())
		JumpAction = IA_Jump.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Run
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Run.IA_Run'"));
	if (IA_Run.Succeeded())
		RunAction = IA_Run.Object;


	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Fire
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (IA_Fire.Succeeded())
		FireAction = IA_Fire.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_GrenadeGun
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_GrendadeGun.IA_GrendadeGun'"));
	if (IA_GrenadeGun.Succeeded())
		ChangeGrenadeGunAction = IA_GrenadeGun.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SniperGun
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SniperGun.IA_SniperGun'"));
	if (IA_SniperGun.Succeeded())
		ChangeSniperGunAction = IA_SniperGun.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SniperAction
	(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Sniper.IA_Sniper'"));
	if (IA_SniperAction.Succeeded())
		SniperAction = IA_SniperAction.Object;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Initiating Speed 
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(PlayerInputContext, 0);
	}

	//1. Create Sniper Widget Instance
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	//2. Create Normal Aim UI CrossHair Intance
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	//3. Regist Normal Aim UI
	_crosshairUI->AddToViewport();

	//Setting First Gun = Sniper
	ChangeToSniperGun();
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

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATPSPlayer::MoveRight);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATPSPlayer::MoveForward);
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATPSPlayer::InputJump);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATPSPlayer::InputFire);
		EnhancedInputComponent->BindAction(ChangeGrenadeGunAction, ETriggerEvent::Triggered, this, &ATPSPlayer::ChangeToGrenadeGun);
		EnhancedInputComponent->BindAction(ChangeSniperGunAction, ETriggerEvent::Triggered, this, &ATPSPlayer::ChangeToSniperGun);
		EnhancedInputComponent->BindAction(SniperAction, ETriggerEvent::Started, this, &ATPSPlayer::SniperAim);
		EnhancedInputComponent->BindAction(SniperAction, ETriggerEvent::Completed, this, &ATPSPlayer::SniperAim);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ATPSPlayer::InputRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ATPSPlayer::InputRun);

	}
}

void ATPSPlayer::MoveRight(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPSPlayer::MoveForward(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);


		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		float delta = GetWorld()->GetDeltaSeconds();
		float speed = 3.0f;
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X* delta*speed);
	}
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

void ATPSPlayer::InputJump(const FInputActionValue& Value)
{
	Jump();
}

void ATPSPlayer::InputFire(const FInputActionValue& Value)
{
	if (bUsingGrenadeGun)
	{
		FTransform FirePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, FirePosition);
	}
	else //Using SniperGun
	{
		//LineTrace Start Position
		FVector startPos = tpsCamComp->GetComponentLocation();
		// LineTrace End Position
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		//Parameter LineTrace collision info
		FHitResult hitInfo;
		// Parmeter setting collsion option 
		FCollisionQueryParams params;
		// except self in collsion
		params.AddIgnoredActor(this);
		//LineTrace collision sorting using Channel Filter(collsionInfo, startPoint, endPoint, sort Channel, collision Option)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		//When LineTrace Hit
		if (bHit)
		{
			//collide process -> Play bullet fragile effect

			//bullet fragilss transform
			FTransform bulletTrans;
			//Allocate collision position
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// Create Bullet Fragiles effect Instance
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

			auto hitComp = hitInfo.GetComponent();
			//1. if component has physics option
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				//2. Need Force And Direction 
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass()*500000;

				//3. Give Force
				hitComp->AddForce(force);
			}

			//Is Enemy?
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();

			}
		}
	}
	
}

void ATPSPlayer::InputRun()
{
	auto movement = GetCharacterMovement();
	//if Player Walking Mode
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}

void ATPSPlayer::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim()
{
	//Do Not approch if not sniperMode
	if (bUsingGrenadeGun)
	{
		return;
	}
	//Input process
	if (bSniperAim == false)
	{
		//1. Active Sniper Aim mode
		bSniperAim = true;
		//2. Regist Sniper Aim UI
		_sniperUI->AddToViewport();
		//3. Setting Camera Field of Vew
		tpsCamComp->SetFieldOfView(45.0f); 
		//4. Remove Normal Aim UI
		_crosshairUI->RemoveFromParent();
	}
	else
	{
		//1. Active Sniper Aim mode
		bSniperAim = false;
		//2. Regist Sniper Aim UI
		_sniperUI->RemoveFromParent();
		//3. Setting Camera Field of Vew
		tpsCamComp->SetFieldOfView(90.0f);
		//4. Regist Normal Aim UI
		_crosshairUI->AddToViewport();
	}
}



