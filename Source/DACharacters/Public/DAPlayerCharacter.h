// Copyright Raioix & Raiocore. All Rights Reserved 2026.

#pragma once

#include "CoreMinimal.h"
#include "DACharacterBase.h"
#include "DAPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class DACHARACTERS_API ADAPlayerCharacter : public ADACharacterBase
{
	GENERATED_BODY()
	

private:
	
#pragma region Camera
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	bool bInFirstPerson = true;
	
#pragma endregion

#pragma region Input
	
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCameraPerspective;
	
#pragma endregion

protected:

	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PlayerJump();
	void TogglePerspective();
	
public:

	ADAPlayerCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
	FORCEINLINE UCameraComponent* GetThirdPersonCamera() const { return ThirdPersonCamera; }
	
};

