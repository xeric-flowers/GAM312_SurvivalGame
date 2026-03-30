// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// M3: include ArrowComponent
#include "Components/ArrowComponent.h" 
#include "BuildingPart.generated.h"

UCLASS()
class GAM312_SURVIVALGAME_API ABuildingPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	// M3: When building, each material will now have same pivot points for rotation purposes 
	UPROPERTY(EditAnywhere)
		UArrowComponent* PivotArrow;


};
