// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initial setup of camera component.
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// Attaching camera to the character mesh and head bone
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	// Pawn shares rotation with controller. ex. Being able to look down and see player's body
	PlayerCamComp->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	// Set timer in BeginPlay so it starts ticking when game starts
	FTimerHandle StatsTimerHandle;
	// Call DecreaseStats Function every 2 seconds
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);	
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	// Action event bind action not axis
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);		
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);



}

// Called to establish axis 
// Input tells us if value is positive or negative, which would make player move forward or backwards
void APlayerChar::MoveForward(float axisValue)
{
	//Set up variables
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);	
	AddMovementInput(Direction, axisValue);															
}

// Similar to MoveForward but with different axis to move right or left
void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);	
	AddMovementInput(Direction, axisValue);
}

// Allows player to jump based on if spacebar is pressed
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

// Stops jump once spacebar is not pressed 
void APlayerChar::StopJump()
{
	bPressedJump = false;
}

// ** Interact with environment, will set up later
void APlayerChar::FindObject()
{

}

// Call functions to adjust Stats in either direction, if negative value then stat subtracts, if positive value then stat adds
// Stop stats from exceeding 100 by adding if statements to the three stat functions
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

// 
void APlayerChar::DecreaseStats()
{
	// Subtract 1 Hunger Stat every interval, when Hunger is greater than 0
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}
	// Regeneration of Stamina
	SetStamina(10.0f);

	// Once Hunger Stat is 0, Health Stat will decrease by 3
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}

}

