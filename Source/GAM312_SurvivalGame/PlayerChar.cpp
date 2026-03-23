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

	// M2: Setting number of elements to 3
	ResourcesArray.SetNum(3);
	// M2: Setting element to name of the appropriate resource
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
}

// M1: Called to establish axis 
// M1: Input tells us if value is positive or negative, which would make player move forward or backwards
void APlayerChar::MoveForward(float axisValue)
{
	// M1: Set up variables
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
	FHitResult HitResult;
	// M2: Grab start location which is the same as the camera location
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	// M2: Direction is 800 units ahead of the camera's current location
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	// M2: Line trace will ignore player character
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	// M2: Allows us to see complex collisions
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	// M2: if statement when get world passes these 
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		// M2: If HitResult.GetActor is equal to AResource_M, then it sets the "HitResource" variable
		AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

		// M2: Add if statement for stamina, if player has 5 value, then it will subtract 5 stamina from it
		if (Stamina > 5.0f)
		{
			// M2: Check if resource is valid, otherwise if hit something other than resource, it might crash editor
			if (HitResource)
			{
				// M2: Get the "hitName" from the resource that player just collected
				FString hitName = HitResource->resourceName;
				// M2: Get the "resourceValue" by checking the resource amount from the resource player just collected
				int resourceValue = HitResource->resourceAmount;

				// M2: From the "HitResource" we are setting its "totalResource" by subtracting its original "totalResource" with the "resourceValue"
				HitResource->totalResource = HitResource->totalResource - resourceValue;

				// M2: If total value is greater than resource value, then give resource info (value and name)
				if (HitResource->totalResource > resourceValue)
				{
					GiveResource(resourceValue, hitName);

					// M2: Check if player hit a resource
					check(GEngine != nullptr);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));
					
					// M2: Spawns in the decal every time hitdecal is called
					// M2: Using GameplayStatics library, set size decal, grab location of hit result, rotation of decal, its lifespan
					UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

					SetStamina(-5.0f);
				
				
				}
				// M2: Once there is no resource value left, the resource is destroyed
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

// M2: Call functions to adjust Stats in either direction, if negative value then stat subtracts, if positive value then stat adds
// M2: Stop stats from exceeding 100 by adding if statements to the three stat functions
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

void APlayerChar::DecreaseStats()
{
	// M2: Subtract 1 Hunger Stat every interval, when Hunger is greater than 0
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}
	// M2: Regeneration of Stamina
	SetStamina(10.0f);

	// M2: Once Hunger Stat is 0, Health Stat will decrease by 3
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}

}

// M2: Determines which resource player is hitting and add info from that
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	// M2: If resourceType (name) is equal to a resource name,
	// M2: Then we are adding the value from that resource and passing it
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

