// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnyWhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;


	//Gun Sk_Mesh
	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//Bullet Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	bool bUsingGrenadeGun = true;

	//Input Setting
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* PlayerInputContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* MoveForwardAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* MoveRightAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* LookUpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* TurnAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* FireAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* SniperAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* ChangeGrenadeGunAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* ChangeSniperGunAction;

	void MoveRight(const FInputActionValue& Value);
	void MoveForward(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputFire(const FInputActionValue& Value);

	//bool using grended gun
	bool bUsingGranadeGun = true;

	void ChangeToGrenadeGun();
	void ChangeToSniperGun();

	//Sniper Aim
	void SniperAim();

	//Sniper Aim widget Factory
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	//SniperWidgetInstance
	class UUserWidget* _sniperUI;


	//is Aiming
	bool bSniperAim = false;

	// Bullet Fragile Effect Factory
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	//Normal Aim CrosshairUI widget
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	//Cross Hair Instance
	class UUserWidget* _crosshairUI;
	
	//SnimperGunStaticMesh
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperGunComp;

};
