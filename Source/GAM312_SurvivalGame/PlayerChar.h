// Fill out your copyright notice in the Description page of Project Settings.
// M1: Note: Header files are used to plan your class, define functions that you will later use in your cpp file

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// M1: Includes the library of camera component to reference elements like adding multiple cameras 
#include "Camera/CameraComponent.h"
// M2: Add access to our resources
#include "Resource_M.h"
// M2: Add gameplay statics library
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_SURVIVALGAME_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// M1: Sets default values for this character's properties
	APlayerChar();

protected:
	// M1: Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// M1: Called every frame
	virtual void Tick(float DeltaTime) override;

	// M1: Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float axisValue);

	UFUNCTION()
	void MoveRight(float axisValue);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void FindObject();

	// M1: Setting to VisibleAnywhere will make camera component visible in Blueprint but not modify it
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	// M2: Creating Player Stats variables, establishing health, hunger, and stamina, all set at 100
	// M2: "UPROPERTY" establishes the variable, 
	// M2: "EditAnywhere" give visibility and ability to change within class
	// M2: "BlueprintReadWrite" allows changes within blueprints, making it more accessible to adjust and test
	// M2: "Player Stats" Category will show up as a section on the blueprint's class defaults 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;

	// M2: Set up integers for resources (Wood, Stone, and Berries)
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// M2: Set up T Array for all resources corresponding to the string array for the names of the resources. 
	// M2: Ex. wood is 0, Stone is 1, and Berry is 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// M2: Resource array needs to match the resource name array
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	// M2: Add new property and call it hitDecal
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// M3: Variables for building mechanics
	// M3: Set up array to store information of how many building part types we have
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingSupplies")
		TArray<int> BuildingArray;

	// M3: Boolean to determine if actively building or not
	UPROPERTY()
		bool isBuilding;

	// M3: Select children when we are spawning objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;

	// M3: whichever object we spawn, we set it to spawned part blueprint
	UPROPERTY()
			ABuildingPart* spawnedPart;

	// M4: UProperty for player widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* playerUI;

	// M2: Blueprint callable functions that sets Health, Hunger, and Stamina. 
	// M2: A timer is added for decreasing the stats
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);

	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	UFUNCTION(BlueprintCallable)
		void DecreaseStats();

	UFUNCTION()
		void GiveResource(float amount, FString resourceType);


	// M3: Set up functions for Building Mechanics
	// M3: Pass through object amounts and names to determine what object is being built by comparing strings
	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);

	UFUNCTION()
		void RotateBuilding();


};
