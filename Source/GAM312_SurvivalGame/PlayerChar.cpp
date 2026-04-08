// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// M1: Sets default values
APlayerChar::APlayerChar()
{
	// M1: Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// M1: Initial setup of camera component.
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// M1: Attaching camera to the character mesh and head bone
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// M1: Pawn shares rotation with controller. ex. Being able to look down and see player's body
	PlayerCamComp->bUsePawnControlRotation = true;

	// M3: Set Building array to 3
	BuildingArray.SetNum(3);

	// M2: Setting number of elements to 3 and set element names to appropriate resource
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

}

// M1: Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// M2: Set timer in BeginPlay so it starts ticking when game starts
	FTimerHandle StatsTimerHandle;
	// M2: Call DecreaseStats Function every 2 seconds
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);

}

// M1: Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// M4: Update player widget health, hunger, and stamina bars every tick
	playerUI->UpdateBars(Health, Hunger, Stamina);

	/* M3: Set up tick event for building mechanic.
	       If we are actively building and if spawned objects are valid, 
	       then we set up a trace that is 400 units ahead from character to set the actors location to place the object.
	       While tick is active, object will follow camera movement 400 units ahead from player, 
	       until we click again, then object is placed wherever player is pointing. Then event tick will stop.*/
	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}
}

// M1: Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	// M1: Action event bind action not axis
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	// M3: Bind rotate building action
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);
}

// M1: Called to establish axis 
//     Input tells us if value is positive or negative, which would make player move forward or backwards
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

// M1: Similar to MoveForward but with different axis to move right or left
void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

// M1: Allows player to jump based on if spacebar is pressed
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

// M1: Stops jump once spacebar is not pressed 
void APlayerChar::StopJump()
{
	bPressedJump = false;
}

// M2: Set up line trace
void APlayerChar::FindObject()
{
	// M2: Establish HitResult Variable
	//     Grab start location which is the same as the camera location
	//     Direction is 800 units ahead of the camera's current location
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	// M2: Line trace parameters that will ignore player character and allow complex collisions to be seen
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	// M3: If Building mode is not active, then line trace (Moved line trace if statements within is building statement)
	if (!isBuilding)
	{
		// M2: If line trace hit result matches a valid resource actor, by comparing strings, set "HitResource" variable,
        //     with enough stamina, collect valid resources and update total value, decrease stamina, 
        //     and delete resource if value is depleted
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			if (Stamina > 5.0f)
			{
				// M2: Validate resource by getting the "hitName"  and "resourceValue" from collected resource and
				//     subtract "resourceValue" from original "totalResource" of the newly collected resource to update total
				if (HitResource)
				{
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					HitResource->totalResource = HitResource->totalResource - resourceValue;

					// M2: Collect resource if resource still has value, and give info (value and name), spawn decal onto
					//     resource mesh, and subtract 5 stamina from player. 
					//     Once there is no resource value left, then resource is destroyed.
					if (HitResource->totalResource > resourceValue)
					{
						GiveResource(resourceValue, hitName);

						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						// M2: Using GameplayStatics library, set size, location (same as hit result), rotation, and lifespan of decal
						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						SetStamina(-5.0f);
					}
					else
					{
						HitResource->Destroy();
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}
		}
	}

	else
	{
		isBuilding = false;
	}

}




// M2: Call functions to adjust Stats in either direction. Negative values subtracts stats, while positive values increases stats
//     Add if statements to stop stats from exceeding 100 
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}

void APlayerChar::SetHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}

void APlayerChar::SetStamina(float amount)
{
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}

// M2: Update player stats at every interval
//     Subtract 1 Hunger, when Hunger is greater than 0
//     Regeneration of Stamina by 10
//     Once Hunger Stat is 0, Health Stat will decrease by 3
void APlayerChar::DecreaseStats()
{
	
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	SetStamina(10.0f);

	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}

}

// M2: Determines which resource that the player is hitting and add info from resource.
//     If resourceType (name) is equal to a resource name,
//     Then we are adding the value from that resource and passing it
void APlayerChar::GiveResource(float amount, FString resourceType)
{

	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

// M3: Building

// M3: Update resource values when building objects, by subtracting resource amounts from resource array
//     and add 1 to the Building array if the object (wall, floor, ceiling) was built
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}

		}
	}
}

void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false;
	}
}

void APlayerChar::RotateBuilding()
{
	if (isBuilding)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

