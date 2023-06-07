// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyFSM.h"
#include "Player/TPSPlayer.h"
#include "Character/Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include "Game/TPSProject.h"
#include "Action/EnemyAnim.h"
#include <NavigationSystem.h>

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
	
	//Allocate UEnemyAnim*
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	//Allocate AIController
	ai = Cast<AAIController>(me->GetController());
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
		AttackState();
		break;
	case EEnemyState::Damage:
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

		anim->animState = mState;


		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}

}

void UEnemyFSM::MoveState()
{
	//1. Target Destination
	FVector destination = target->GetActorLocation();
	//2. Direction
	FVector dir = destination - me->GetActorLocation();
	
	//Get NavigationSystem Instance
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//Serach Destination Path Finder
	FPathFindingQuery query;
	FAIMoveRequest req;
	//Range of request
	req.SetAcceptanceRadius(10);
	req.SetGoalLocation(destination);
	//Create Queary for path Finder
	ai->BuildPathfindingQuery(req, query);

	//Get PathFinding Result
	FPathFindingResult r = ns->FindPathSync(query);

	//Get result of Path Finding
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		//Move Random Position
		auto result = ai->MoveToLocation(randomPos);
		//if arrived in destination
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			//Get New Random Position
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}
	//Chage to AttackState closer to target
	//1. if distance in range
	if (dir.Size() < attackRange)
	{
		//Pause PathFinder
		ai->StopMovement();
		//2. Change to Attack State
		mState = EEnemyState::Attack;

		anim->animState = mState;
		anim->bAttackPlay = true;
		currentTime = attackDelayTime;

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
		//PRINT_LOG(TEXT("ATTACK!!"));
		currentTime = 0;
		anim->bAttackPlay = true;
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
		anim->animState = mState;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
 }

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	if (anim->bDiedone == false)
	{
		return;
	}
	// P = P0 + vy
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}

	anim->animState = mState;
}

void UEnemyFSM::OnDamageProcess()
{
	// -hp;
	hp--;

	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;

		//Play Damage Animation
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), 0);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;

		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		anim->PlayDamageAnim(TEXT("Die"));

		
	}
	UE_LOG(LogTemp, Warning, TEXT("Enmey Damage"));

	anim->animState = mState;
	ai->StopMovement();

}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool results = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;

	return results;
}

