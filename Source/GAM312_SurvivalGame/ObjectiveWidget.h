// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAM312_SURVIVALGAME_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// M5: Update objective text in widget when player collects resources or builds objects.
	UFUNCTION(BlueprintImplementableEvent)
		void UpdatematOBJ(float matsCollected);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdatebuildOBJ(float objectsBuilt);
	

};
