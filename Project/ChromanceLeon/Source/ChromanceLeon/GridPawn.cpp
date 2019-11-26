// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawn.h"
#include "Camera//CameraComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AGridPawn::AGridPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	//UCameraComponent* camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	/*camera->SetupAttachment(RootComponent);
	camera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	camera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));*/

	meshComponent->SetupAttachment(RootComponent);
	float scale = 100.0f;
	meshComponent->SetRelativeScale3D(FVector(scale, scale, scale));
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Create"));
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

