// Fill out your copyright notice in the Description page of Project Settings.
// note: Header files are used to plan your class, define functions that you will later use in your cpp file

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// Includes the library of camera component to reference elements like adding multiple cameras 
#include "Camera/CameraComponent.h"
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



};
