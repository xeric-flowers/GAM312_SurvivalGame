// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// M2: Text render added for testing
#include "Components/TextRenderComponent.h" 
// M2: Implements static mesh
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312_SURVIVALGAME_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// M2: Sets default values for this actor's properties
	AResource_M();

protected:
	// M2: Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// M2: Called every frame
	virtual void Tick(float DeltaTime) override;

	// M2: F string "resourceName" is defaulted to Wood. "EditAnywhere" is added to access through the details panel
	UPROPERTY(EditAnywhere)
		FString resourceName = "Wood";

	// M2: "resourceAmount" is going to give us this amount every time player hits this resource
	UPROPERTY(EditAnywhere)
		int resourceAmount = 5;

	// M2: "totalResource" is the total amount that the resource has before it gets depleted
	UPROPERTY(EditAnywhere)
		int totalResource = 100;

	// M2: temp text sets text render to the text itself
	UPROPERTY()
		FText tempText;

	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ResourceNameTxt;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

};
