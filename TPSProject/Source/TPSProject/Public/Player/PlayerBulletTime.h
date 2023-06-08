// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerBaseComponent.h"
#include "Game/TPSProject.h"
#include "PlayerBulletTime.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerBulletTime : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerBulletTime();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	//Bullet Time
	UPROPERTY()
	bool bBulletTime = false;
};
