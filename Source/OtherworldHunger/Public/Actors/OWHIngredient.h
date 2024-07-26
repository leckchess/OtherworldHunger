// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OWHInteractableInterface.h"
#include "GameFramework/Actor.h"
#include "OWHIngredient.generated.h"

class USphereComponent;

UCLASS(BlueprintType)
class OTHERWORLDHUNGER_API AOWHIngredient : public AActor, public IOWHInteractableInterface
{
	GENERATED_BODY()

public:
	AOWHIngredient();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* IngredientMesh;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractSphere;

protected:
	virtual void BeginPlay() override;

	virtual AActor* Interact_Implementation(APawn* InstigatorPawn) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText IngredientName;

	//GETTERS AND SETTERS
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintGetter)
	FText GetIngredientName() const;
	UFUNCTION(BlueprintSetter)
	void SetIngredientName(const FText& Ingredient);
	UFUNCTION(BlueprintSetter)
	void SetIngredientMesh(UStaticMeshComponent* IngredientMeshComp);

	virtual void Tick(float DeltaTime) override;
};
