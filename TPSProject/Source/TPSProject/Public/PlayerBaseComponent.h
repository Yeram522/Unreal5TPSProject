// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBaseComponent()
	{
		//No Tick Func 
		PrimaryComponentTick.bCanEverTick = false;
	};

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerinputComponent) {};
public:
	UPROPERTY()
	ATPSPlayer* me;

	UPROPERTY()
	UCharacterMovementComponent* moveComp;
};
