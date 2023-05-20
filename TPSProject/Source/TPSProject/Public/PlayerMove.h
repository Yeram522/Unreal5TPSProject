// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	//Camera
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* LookUpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* TurnAction;

	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	//Player Move
	//Walking Speed
	UPROPERTY(EditAnyWhere, Category = PlayerSetting)
	float walkSpeed = 200;
	//Run Speed
	UPROPERTY(EditAnyWhere, Category = PlayerSetting)
	float runSpeed = 600;

	//Move-Action
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* MoveForwardAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* MoveRightAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* RunAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* JumpAction;

	//Move-BindingFunc
	void MoveRight(const FInputActionValue& Value);
	void MoveForward(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputRun();
};
