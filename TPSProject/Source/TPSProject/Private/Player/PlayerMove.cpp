// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerMove.h"
#include "EnhancedInputComponent.h"
#include <Camera/CameraComponent.h>

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;

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

}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	// Initiating Speed 
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UPlayerMove::Turn(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (me->Controller != nullptr)
	{
		float delta = GetWorld()->GetDeltaSeconds();
		float speed = 3.0f;
		// add yaw and pitch input to controller
		me->AddControllerYawInput(LookAxisVector.X * delta * speed);
	}
}

void UPlayerMove::LookUp(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (me->Controller != nullptr)
	{
		float delta = GetWorld()->GetDeltaSeconds();
		float speed = 5.0f;
		// add yaw and pitch input to controller
		me->AddControllerPitchInput(-LookAxisVector.Y * delta * speed);
	}

}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &UPlayerMove::LookUp);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);

		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &UPlayerMove::MoveRight);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &UPlayerMove::MoveForward);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &UPlayerMove::InputJump);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &UPlayerMove::InputRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &UPlayerMove::InputRun);
	}
}

void UPlayerMove::MoveRight(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (me->Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = me->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		me->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UPlayerMove::MoveForward(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (me->Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = me->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);


		// add movement 
		me->AddMovementInput(ForwardDirection, MovementVector.Y);
	}
}



void UPlayerMove::InputJump(const FInputActionValue& Value)
{
	me->Jump();
}

void UPlayerMove::InputRun()
{
	auto movement = moveComp;
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