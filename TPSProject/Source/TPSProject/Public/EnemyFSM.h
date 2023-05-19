// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//Generated State
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//State Parameter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = FSM)
	EEnemyState mState = EEnemyState::Idle;

	//Idle State
	void IdleState();
	//Move State
	void MoveState();
	//Attack State
	void AttackState();
	//Damage State
	void DamageState();
	//DieState
	void DieState();


	//Delay Time
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelayTime = 2;
	//Current Time
	float currentTime = 0;

	//Target
	UPROPERTY(VisibleAnywhere, Category= FSM)
	class ATPSPlayer* target;

	//My Actor
	UPROPERTY()
	class AEnemy* me;

	//Attack Range
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;

	//Attack Idle Time
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackDelayTime = 2.0f;

	//Demage Alarm Event Function
	void OnDamageProcess();

	//HP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;

	//Damage Delay Time
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;

	//Destroy Speed
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;

	//Using Animation BluePrint
	UPROPERTY()
	class UEnemyAnim* anim;
};
