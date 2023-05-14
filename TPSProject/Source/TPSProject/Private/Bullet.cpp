// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//regist collision
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	//setting collision file
	collisionComp->SetCollisionProfileName(TEXT("BlocAll"));
	//set collision size
	collisionComp->SetSphereRadius(13);
	//regist root
	RootComponent = collisionComp;


	

	//regist bodyMesh
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	//set parentComponent
	bodyMeshComp->SetupAttachment(collisionComp);
	//inActive Collision
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//set scale
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	ConstructorHelpers::FObjectFinder<UStaticMesh>
		TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (TempMesh.Succeeded())
	{
		bodyMeshComp->SetStaticMesh(TempMesh.Object);
		bodyMeshComp->SetRelativeLocation(FVector(0, 0, -12.5f));
		UE_LOG(LogTemp, Log, TEXT("Load"));
	}

	//movemnet component
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovemnetComp"));
	//set updating component
	movementComp->SetUpdatedComponent(collisionComp);

	//starting Speed
	movementComp->InitialSpeed = 5000;
	//Max Speed
	movementComp->MaxSpeed = 5000;
	//bool Bounce
	movementComp->bShouldBounce = true;
	//Bounced value
	movementComp->Bounciness = 0.3f;

	//LifeSpan
	InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, FTimerDelegate::CreateLambda([this]()->void
		{
			Destroy();
		}), 2.0f, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent, const FProperty* PropertyThatChanged)
{
	//Check if speed changed
	if (PropertyChangedEvent.GetPropertyName() == TEXT("Speed"))
	{
		//Set speed value to projectile movement component
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}

