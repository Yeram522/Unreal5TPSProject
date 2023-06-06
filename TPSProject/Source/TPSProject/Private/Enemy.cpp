// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//1. Load SkeletalMesh Data
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ZombotMike/Mesh/SM_Zombot_Mikey.SM_Zombot_Mikey'"));
	//1-1. If Success Data Load
	if (tempMesh.Succeeded())
	{
		//1-2. Allocate Data
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1-3. Set Mesh Location And Rotation
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	//Add EnemyFSM Component
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	//Add Enemy Animation BluePrint
	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
		UE_LOG(LogTemp, Warning, TEXT("AnimInstance Loaded"));
	}

	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

