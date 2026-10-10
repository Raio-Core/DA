// Copyright Raioix & Raiocore. All Rights Reserved 2026.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DACharacterBase.generated.h"

UCLASS()
class DACHARACTERS_API ADACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

