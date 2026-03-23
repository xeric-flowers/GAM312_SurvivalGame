// Fill out your copyright notice in the Description page of Project Settings.
// note: Header files are used to plan your class, define functions that you will later use in your cpp file

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// Includes the library of camera component to reference elements like adding multiple cameras 
#include "Camera/CameraComponent.h"
// Add access to our resources
#include "Resource_M.h"
// Add gameplay statics library
#include "Kismet/GameplayStatics.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_SURVIVALGAME_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
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

	// Setting to VisibleAnywhere will make camera component visible in Blueprint but not modify it
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	// Creating Player Stats variables, establishing health, hunger, and stamina, all set at 100
	// "UPROPERTY" establishes the variable, 
	// "EditAnywhere" give visibility and ability to change within class
	// "BlueprintReadWrite" allows changes within blueprints, making it more accessible to adjust and test
	// "Player Stats" Category will show up as a section on the blueprint's class defaults 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;

	// Set up integers for resources (Wood, Stone, and Berries)
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// Set up T Array for all resources corresponding to the string array for the names of the resources. 
	// Ex. wood is 0, Stone is 1, and Berry is 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// Resource array needs to match the resource name array
	UPROPERTY(EditAnywhere, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	// Add new property and call it hitDecal
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	// Blueprint callable functions that sets Health, Hunger, and Stamina. 
	// A timer is added for decreasing the stats
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









};
