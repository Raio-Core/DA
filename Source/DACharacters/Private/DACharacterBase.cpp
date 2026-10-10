// Copyright Raioix & Raiocore. All Rights Reserved 2026.


#include "DACharacterBase.h"


// Sets default values
ADACharacterBase::ADACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ADACharacterBase::CanCharacterJump() const
{
	return CanJump();
}

void ADACharacterBase::HasJumped()
{
	
}

// Called every frame
void ADACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


