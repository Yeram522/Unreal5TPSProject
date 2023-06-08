// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerFire.h"
#include "EnhancedInputComponent.h"
#include "AI/EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Player/Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include "Action/PlayerAnim.h"
#include "CableComponent.h"

UPlayerFire::UPlayerFire()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Get Bullet Sound
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound
	(TEXT("/Game/SniperGun/Rifle.Rifle"));
	if (tempSound.Succeeded())
		bulletSound = tempSound.Object;

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

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;
	CableComp = me->CableComp;

	//1. Create Sniper Widget Instance
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	//2. Create Normal Aim UI CrossHair Intance
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	//3. Regist Normal Aim UI
	_crosshairUI->AddToViewport();

	//Setting First Gun = Sniper
	ChangeToSniperGun();
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UPlayerFire::InputFire);
		EnhancedInputComponent->BindAction(ChangeGrenadeGunAction, ETriggerEvent::Triggered, this, &UPlayerFire::ChangeToGrenadeGun);
		EnhancedInputComponent->BindAction(ChangeSniperGunAction, ETriggerEvent::Triggered, this, &UPlayerFire::ChangeToSniperGun);
		EnhancedInputComponent->BindAction(SniperAction, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
		EnhancedInputComponent->BindAction(SniperAction, ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);


	}
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isGrappling)
	{
		CableComp->SetVisibility(false);
	}
}

void UPlayerFire::InputFire(const FInputActionValue& Value)
{
	//Play Attack Animation
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	if (bUsingGrenadeGun) //Using grapping  Gun
	{
		/*FTransform FirePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, FirePosition);*/

		//LineTrace Start Position
		FVector startPos = tpsCamComp->GetComponentLocation();
		// LineTrace End Position
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		//Parameter LineTrace collision info
		FHitResult hitInfo;
		// Parmeter setting collsion option 
		FCollisionQueryParams params;
		// except self in collsion
		params.AddIgnoredActor(me);
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
			
			CableComp->SetVisibility(true);

			

			FComponentReference ref;
			ref.OtherActor = hitInfo.GetActor();
			CableComp->AttachEndTo = ref;
			CableEndLocation = ref.OtherActor->GetActorTransform().InverseTransformPosition(bulletTrans.GetLocation());

			CableComp->EndLocation = CableEndLocation;

			isGrappling = true;

			FVector force = -hitInfo.ImpactNormal * me->GetActorLocation() * 5000;

			me->GetCharacterMovement()->AddForce(force);
			me->AddMovementInput(force, 5000);
		}
	}
	else //Using SniperGun
	{
		UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

		//Play Camera Shake
		auto controller = GetWorld()->GetFirstPlayerController();
		controller->PlayerCameraManager->StartCameraShake(cameraShake);

		//LineTrace Start Position
		FVector startPos = tpsCamComp->GetComponentLocation();
		// LineTrace End Position
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 2500;
		//Parameter LineTrace collision info
		FHitResult hitInfo;
		// Parmeter setting collsion option 
		FCollisionQueryParams params;
		// except self in collsion
		params.AddIgnoredActor(me);
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
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;

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

void UPlayerFire::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void UPlayerFire::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void UPlayerFire::SniperAim()
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