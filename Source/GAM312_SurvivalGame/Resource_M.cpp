// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource_M.h"

// M2: Sets default values
AResource_M::AResource_M()
{
 	// M2: Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// M2: Set default subObjects for the newly added UProperties in header file.
	ResourceNameTxt = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Mesh;

	// M2: Attach text render component to the mesh
	ResourceNameTxt->SetupAttachment(Mesh);
}

// M2: Called when the game starts or when spawned
void AResource_M::BeginPlay()
{
	Super::BeginPlay();

	// M2: Grabs the text from the string of the resource that is set from details panel 
	tempText = tempText.FromString(resourceName);

	// M2: Then sets text render to the new text
	ResourceNameTxt->SetText(tempText);
	
}

// M2: Called every frame
void AResource_M::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

