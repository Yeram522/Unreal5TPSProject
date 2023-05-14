// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include "TPSProject.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// Search ATPSPlayer Target in World
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	// Casting By ATPSPlayer Type
	target = Cast<ATPSPlayer>(actor);
	// Get My Actor
	me = Cast<AEnemy>(GetOwner());
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	//1. Current Time
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. CurrentTime > IdleDelayTime
	if (currentTime > idleDelayTime)
	{
		//3. Change Move State
		mState = EEnemyState::Move;
		//initial currenTime
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	//1. Target Destination
	FVector destination = target->GetActorLocation();
	//2. Direction
	FVector dir = destination - me->GetActorLocation();
	//3. Move to Direction
	me->AddMovementInput(dir.GetSafeNormal());

	//Chage to AttackState closer to target
	//1. if distance in range
	if (dir.Size() < attackRange)
	{
		//2. Change to Attack State
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	//goal: Attack Per Time
	//1. time going
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2. Time to Attack
	if (currentTime > attackRange)
	{
		PRINT_LOG(TEXT("ATTACK!!"));
	}
	//2. initiate current Time
	currentTime = 0;

	//goal : Change To move State if get target out attack range
	//1. target and enemey distance
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	//2. Target get out Attack Range
	if (distance > attackRange)
	{
		//3. change to move state
		mState = EEnemyState::Move;
	}
 }

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
	}
}

void UEnemyFSM::DieState()
{
	// P = P0 + vy
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	// -hp;
	hp--;

	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;

		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}

