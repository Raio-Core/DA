// Copyright Raioix & Raiocore. All Rights Reserved 2026.

#include "DAPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ADAPlayerCharacter::ADAPlayerCharacter()
{

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;
	ThirdPersonCamera->Deactivate();
	ThirdPersonCamera->bAutoActivate = false;
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), "head");
	FirstPersonCamera->SetRelativeRotation(FRotator(0, -90, 90));
	FirstPersonCamera->SetRelativeLocation (FVector(15, 20, 2.5));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->Activate();

	
}


void ADAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADAPlayerCharacter::PlayerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADAPlayerCharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADAPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADAPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(ToggleCameraPerspective, ETriggerEvent::Completed, this, &ADAPlayerCharacter::TogglePerspective);
		
		/* Reminder for future implementation
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ADAPlayerCharacter::SprintOn);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADAPlayerCharacter::SprintOff);
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Started, this, &ADAPlayerCharacter::SneakOn);
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Completed, this, &ADAPlayerCharacter::SneakOff);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ADAPlayerCharacter::OnInteract);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &ADAPlayerCharacter::TogglePlayerInventory_Implementation);
		EnhancedInputComponent->BindAction(LeanAction, ETriggerEvent::Triggered, this, &ADAPlayerCharacter::Lean);
		EnhancedInputComponent->BindAction(LeanAction, ETriggerEvent::Completed, this, &ADAPlayerCharacter::Lean);
		*/
	}
}

void ADAPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADAPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADAPlayerCharacter::PlayerJump()
{
	 if (ADACharacterBase::CanCharacterJump() && !GetMovementComponent()->IsFalling())
	 {
		 ADACharacterBase::HasJumped();
	 }
}

void ADAPlayerCharacter::TogglePerspective()
{
	bInFirstPerson = !bInFirstPerson;
	if (!bInFirstPerson)
	{
		FirstPersonCamera->Deactivate();
		ThirdPersonCamera->Activate();
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
		return;
	}
	ThirdPersonCamera->Deactivate();
	FirstPersonCamera->Activate();
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	return;
}