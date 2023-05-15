// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//1. Get My Pawn
	auto ownerPawn = TryGetPawnOwner();

	//2. Casting to Player
	auto player = Cast<ATPSPlayer>(ownerPawn);

	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}

}
