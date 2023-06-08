// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerBulletTime.h"

UPlayerBulletTime::UPlayerBulletTime()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerBulletTime::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerBulletTime::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	bool bIsInAir = me->GetCharacterMovement()->IsFalling();


	float Distance = 0;

	FVector CharacterLocation = me->GetActorLocation();
	FFindFloorResult OutFloorResult;
	me->GetCharacterMovement()->ComputeFloorDist(CharacterLocation, Distance, 200, OutFloorResult, 10);
	Distance = OutFloorResult.FloorDist;

	PRINT_LOG(TEXT("Distance!!!! %f"), Distance);


	if (bIsInAir && Distance > 190)
	{
		bBulletTime = true;
	}
	else if (!bIsInAir)
	{
		bBulletTime = false;
	}
}
