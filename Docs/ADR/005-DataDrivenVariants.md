# ADR-005: Data-Driven Variant Configuration

## Status
✅ Accepted

## Context
The framework supports four gameplay variants (Third-Person, Combat, Platforming, Side-Scrolling) with different characters, input, cameras, movement, and progression. Hardcoding variant logic creates tight coupling and makes adding variants difficult.

## Decision
Drive all variant differences through external data assets (JSON + Data Assets), loaded at runtime by `UDAVariantSubsystem`. Code contains zero variant-specific logic.

## Consequences

### Positive
- **New Variants**: Add by creating data assets + content, no C++ changes
- **Designer Control**: Tweak movement, camera, input, progression without code
- **Hot Reload**: Change variant config without restart (in editor)
- **Testing**: Swap variants in-game for comparison
- **Modding**: External config enables community variants
- **CI/CD**: Validate configs in pipeline (JSON schema)

### Negative
- **Indirection**: One extra lookup at startup/variant switch
- **Validation**: Must validate data at load time (schema, references)
- **Tooling**: Need editor tools for config editing (VariantConfigEditor)
- **Migration**: Config versioning required for updates

### Neutral
- **Code vs Data**: Core systems (movement, camera, input) remain in C++; only parameters externalized

## Implementation

### JSON Config Schema
```json
{
  "VariantName": "ThirdPerson",
  "DisplayName": "Third Person Adventure",
  "CharacterClass": "/Game/ThirdPerson/BP_ThirdPersonCharacter",
  "InputContext": "/Game/Input/IMC_ThirdPerson",
  "StateTree": "/Game/ThirdPerson/ST_ThirdPerson",
  "CameraConfig": "/Game/Config/Cameras/ThirdPersonCamera",
  "MovementConfig": "/Game/Config/Movement/ThirdPersonMovement",
  "ProgressionConfig": "/Game/Config/Progression/ThirdPersonProgression"
}
```

### Data Assets (UDataAsset)
```cpp
// Camera Config
UCLASS(BlueprintType)
class UDACharacterCameraConfig : public UDataAsset
{
    GENERATED_BODY()
public:
    float DefaultDistance = 400.0f;
    float MinDistance = 150.0f;
    float MaxDistance = 600.0f;
    float FOV = 90.0f;
    FVector2D PitchLimits = FVector2D(-80.0f, 80.0f);
    float LagSpeed = 10.0f;
    bool bUsePawnControlRotation = true;
};

// Movement Config
UCLASS(BlueprintType)
class UDACharacterMovementConfig : public UDataAsset
{
    GENERATED_BODY()
public:
    float WalkSpeed = 300.0f;
    float RunSpeed = 600.0f;
    float SprintMultiplier = 2.0f;
    float JumpZVelocity = 700.0f;
    float AirControl = 0.35f;
    // ... dash, wall run, etc.
};
```

## Alternatives Considered
- **C++ Inheritance**: `AThirdPersonCharacter`, `ACombatCharacter` — tight coupling, code duplication
- **Component Composition**: Add/remove components at runtime — complex, hard to debug
- **GameMode Only**: All variant logic in GameMode — violates SRP, hard to test

## References
- `Source/DA/Public/DAVariantSubsystem.h`: Subsystem interface
- `Source/DA/Private/DAVariantSubsystem.cpp`: JSON loading, application
- `Config/Variants/*.json`: Variant configurations
- `Source/DAEditor/Private/VariantConfigEditor.cpp`: Editor validation