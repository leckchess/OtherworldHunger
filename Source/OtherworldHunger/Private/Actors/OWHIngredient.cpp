// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OWHIngredient.h"
#include "Components/SphereComponent.h"

// Sets default values
AOWHIngredient::AOWHIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	IngredientName = FText::FromString("BasicIngredient");
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	InteractSphere->InitSphereRadius(50.0f);
	InteractSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = IngredientMesh;
	InteractSphere->SetupAttachment(RootComponent);

	// Set up collision responses
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AOWHIngredient::OnOverlapBegin);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AOWHIngredient::OnOverlapEnd);
}

void AOWHIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOWHIngredient::BeginPlay()
{
	Super::BeginPlay();
}

AActor* AOWHIngredient::Interact_Implementation(APawn* InstigatorPawn)
{
	return this;
}

void AOWHIngredient::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	}
}

void AOWHIngredient::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Ended"));
}

FText AOWHIngredient::GetIngredientName() const
{
	return IngredientName;
}

void AOWHIngredient::SetIngredientName(const FText& NameOfIngredient)
{
	this->IngredientName = NameOfIngredient;
}


void AOWHIngredient::SetIngredientMesh(UStaticMeshComponent* IngredientMeshComp)
{
	IngredientMesh = IngredientMeshComp;
}
