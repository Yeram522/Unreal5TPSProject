// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class UCameraComponent* tpsCamComp;


	//Gun Sk_Mesh
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;


	//SnimperGunStaticMesh
	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

	UPROPERTY()
	class UCableComponent* CableComp;

	UPROPERTY()
	FVector CableEndLocation;

	UPROPERTY()
	bool isGrappling = false;
public:
	//Bullet Factory
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* FireAction;

	void InputFire(const FInputActionValue& Value);

	//bool using grended gun
	bool bUsingGrenadeGun = true;


	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* SniperAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* ChangeGrenadeGunAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* ChangeSniperGunAction;

	void ChangeToGrenadeGun();
	void ChangeToSniperGun();

	//Sniper Aim
	void SniperAim();

	//is Aiming
	bool bSniperAim = false;

	//Sniper Aim widget Factory
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	//SniperWidgetInstance
	class UUserWidget* _sniperUI;

	// Bullet Fragile Effect Factory
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	//Normal Aim CrosshairUI widget
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	//Cross Hair Instance
	class UUserWidget* _crosshairUI;


	//Camera Shake Variable
	UPROPERTY(EditDefaultsOnly, Category = Cameramotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;


	//Shooting Sound
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;

};
