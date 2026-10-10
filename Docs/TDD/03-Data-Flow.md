# 🔧 Technical Design Document — Data Flow

<div align="center">

![Version](https://img.shields.io/badge/Version-0.1.0-blue?style=for-the-badge&logo=git)
![Status](https://img.shields.io/badge/Status-In%20Development-orange?style=for-the-badge&logo=trello)

[← Module Design](02-Module-Design.md) | [Networking →](04-Networking.md)

</div>

---

## 🔄 End-to-End Data Flow

### Input → Gameplay → Render Pipeline

```mermaid
sequenceDiagram
    autonumber
    actor Player
    participant Platform as Platform Input
    participant EI as Enhanced Input
    participant IM as Input Mapping
    participant IA as Input Action
    participant Char as ADAPlayerCharacter
    participant ASC as AbilitySystemComponent
    participant GE as GameplayEffect
    participant Attr as Attributes
    participant ST as StateTree
    participant Anim as AnimInstance
    participant Mesh as SkeletalMesh
    participant Render as Renderer

    Player->>Platform: Hardware Input
    Platform->>EI: Raw Input Events
    EI->>IM: Process Mapping Contexts
    IM->>IA: Trigger Input Actions
    IA->>Char: Bound Callback (e.g., Move())
    Char->>Char: Calculate Movement Vector
    Char->>Char: AddMovementInput()
    
    alt Ability Input
        Char->>ASC: TryActivateAbility(Tag)
        ASC->>ASC: Check Cost/Cooldown/Tags
        ASC->>GE: Apply GameplayEffect (Cost)
        GE->>Attr: Modify Attributes
        ASC->>Char: Ability Activated Event
        Char->>ST: Send Gameplay Event
        ST->>ST: Evaluate Transitions
        ST->>Char: Execute Task (Montage, Spawn, etc.)
    end
    
    Char->>MoveComp: Velocity/Acceleration
    MoveComp->>MoveComp: Simulate Movement
    MoveComp->>Char: Updated Transform
    Char->>Anim: Update Animation State
    Anim->>Anim: State Machine Evaluation
    Anim->>Mesh: Final Pose (with IK)
    Mesh->>Render: Draw Call
    Render->>Player: Frame Output
```

---

## 📥 Input Pipeline

### Enhanced Input Flow

```mermaid
flowchart TD
    Raw[Raw Input<br/>Keyboard/Mouse/Gamepad] --> Subsys[Input Subsystem<br/>FInputDevice]
    Subsys --> Mapping[Input Mapping Contexts<br/>Priority Stack]
    Mapping --> Action[Input Actions<br/>IA_Move, IA_Jump, etc.]
    Action --> Trigger{Trigger Type}
    Trigger -->|Started| Started[Started Callback]
    Trigger -->|Ongoing| Ongoing[Ongoing Callback<br/>Per Frame]
    Trigger -->|Completed| Completed[Completed Callback]
    Trigger -->|Triggered| Triggered[Triggered Callback<br/>Ongoing + Started]
    Trigger -->|Custom| Custom[Custom Trigger<br/>Charge, Swipe, etc.]
    
    Started --> Mod[Modifiers]
    Ongoing --> Mod
    Completed --> Mod
    Triggered --> Mod
    Custom --> Mod
    
    Mod --> DeadZone[Dead Zone]
    Mod --> Scale[Scale/Sensitivity]
    Mod --> Swipe[Swipe Detection]
    Mod --> Hold[Hold Detection]
    Mod --> Axis2D[Axis2D Processing]
    
    DeadZone --> Bind[Bound Callback]
    Scale --> Bind
    Swipe --> Bind
    Hold --> Bind
    Axis2D --> Bind
    
    Bind --> Char[ADAPlayerCharacter::Callback]
```

### Input Action Data Flow

```cpp
// IA_Move (Axis2D) → Move()
void ADAPlayerCharacter::Move(const FInputActionValue& Value)
{
    // 1. Extract 2D vector
    FVector2D MovementVector = Value.Get<FVector2D>();  // X=Strafe, Y=Forward
    
    // 2. Validate controller
    if (!Controller) return;
    
    // 3. Calculate yaw-relative directions
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    
    // 4. Apply movement input (Y=Forward, X=Right)
    AddMovementInput(ForwardDirection, MovementVector.Y);
    AddMovementInput(RightDirection, MovementVector.X);
}
```

---

## ⚔️ Combat Ability Pipeline

### Gameplay Ability Activation

```mermaid
flowchart TD
    Input[Input Trigger] --> TryActivate[ASC::TryActivateAbility]
    TryActivate --> Check{Can Activate?}
    Check -->|No| Fail[Fail: Cooldown/Cost/Tags]
    Check -->|Yes| Commit[Commit Ability]
    Commit --> Cost[Apply Cost GE\nStamina/Focus]
    Cost --> Tags[Apply/Remove Tags\nState, Blocking]
    Tags --> Activate[Activate Ability\nK2_ActivateAbility]
    Activate --> Logic[Ability Logic\nMontage, Projectile, Effect]
    Logic --> Events[Gameplay Events\nOnHit, OnDamage, etc.]
    Events --> Cue[Gameplay Cue\nVFX/SFX/Camera]
    Cue --> End[End Ability\nK2_EndAbility]
    End --> Cooldown[Start Cooldown]
    Cooldown --> Tags2[Remove Tags]
```

### Damage Calculation Flow

```mermaid
flowchart TD
    Attack[Attack Hits] --> Context[Build FDamageContext]
    Context --> Base[Base Damage\nWeapon + Stats]
    Base --> AttackerMod[Attacker Modifiers\nBuffs, Stance, Combo]
    AttackerMod --> DefenderMod[Defender Modifiers\nArmor, Resistance, Debuffs]
    DefenderMod --> TypeEff[Type Effectiveness\nPhysical vs Armor]
    TypeEff --> Crit{Critical Hit?}
    Crit -->|Yes| CritMult[×1.5 + Crit Damage]
    Crit -->|No| NoCrit[×1.0]
    CritMult --> Variance[±5% Variance]
    NoCrit --> Variance
    Variance --> Min[Max(1, Damage)]
    Min --> Apply[Apply Damage GE]
    Apply --> Health[Modify Health Attribute]
    Health --> Death{Health ≤ 0?}
    Death -->|Yes| Die[OnDeath Event]
    Death -->|No| React[Hit Reaction]
    React --> Cue[Damage Cue\nNumbers, VFX, SFX]
```

---

## 🏃 Movement Pipeline

### Character Movement Tick

```mermaid
flowchart TD
    Tick[Character Tick] --> Input[Process Input\nMove, Jump, Dash]
    Input --> MoveComp[UCharacterMovementComponent::TickComponent]
    MoveComp --> Mode{Movement Mode}
    Mode -->|Walking| Walk[Walk Logic\nAcceleration, Friction]
    Mode -->|Falling| Fall[Fall Logic\nGravity, Air Control]
    Mode -->|Swimming| Swim[Swim Logic\nBuoyancy, Drag]
    Mode -->|Flying| Fly[Fly Logic\nNo Gravity]
    Mode -->|Custom| Custom[Custom Mode\nDash, WallRun]
    
    Walk --> Velocity[Update Velocity]
    Fall --> Velocity
    Swim --> Velocity
    Fly --> Velocity
    Custom --> Velocity
    
    Velocity --> Collision[Sweep Collision]
    Collision --> Hit{Hit Wall/Floor?}
    Hit -->|Yes| Slide[Slide Along Surface]
    Hit -->|No| Move[Move Actor]
    Slide --> Move
    
    Move --> Floor[Update Floor Result]
    Floor --> Anim[Drive Animation\nSpeed, Direction, InAir]
    Anim --> RootMotion[Apply Root Motion\nIf Enabled]
    RootMotion --> Final[Final Transform]
```

### Variant Movement Parameters

| Parameter | TP | Combat | Platforming | SideScroll |
|-----------|-----|--------|-------------|------------|
| **MaxWalkSpeed** | 600 | 500 | 600 | 400 |
| **MaxSprintSpeed** | 1200 | 900 | 1300 | N/A |
| **Acceleration** | 2000 | 2500 | 3000 | 3000 |
| **Deceleration** | 2000 | 3000 | 5000 | 5000 |
| **JumpZVelocity** | 700 | 650 | 800 | 750 |
| **AirControl** | 0.35 | 0.30 | 0.50 | 0.40 |
| **DashDistance** | 500 | 500 | 400 | 300 |
| **DashCooldown** | 1.0s | 1.0s | 0.8s | 0.6s |
| **WallRunSpeed** | 800 | 700 | 800 | N/A |
| **LedgeGrabHeight** | 200 | 150 | 250 | N/A |

---

## 🎭 Animation Pipeline

### Animation Blueprint Data Flow

```mermaid
flowchart LR
    Char[Character] --> Vel[Velocity]
    Char --> Rot[Rotation]
    Char --> Mode[MovementMode]
    Char --> Tags[Gameplay Tags]
    Char --> Attr[Attributes]
    
    Vel --> ABP[Anim Blueprint]
    Rot --> ABP
    Mode --> ABP
    Tags --> ABP
    Attr --> ABP
    
    ABP --> SM[State Machine]
    SM --> Loc[Locomotion State]
    SM --> Air[Airborne State]
    SM --> Crouch[Crouch State]
    SM --> Combat[Combat Additive]
    SM --> Plat[Platforming Additive]
    
    Loc --> Blend[Blend Space 1D/2D]
    Blend --> Pose[Final Pose]
    
    Air --> Pose
    Crouch --> Pose
    
    Combat --> Add[Additive Blend]
    Plat --> Add
    Add --> Pose
    
    Pose --> IK[IK Layer\nFoot, Hand, LookAt]
    IK --> Final[Final Output Pose]
    Final --> Mesh[Skeletal Mesh]
```

### Gameplay Tag → Animation Mapping

| Gameplay Tag | Animation Effect |
|--------------|------------------|
| `State.Moving` | Enable Locomotion blend |
| `State.Sprinting` | Sprint blend space |
| `State.InAir` | Airborne state |
| `State.Crouching` | Crouch state |
| `Ability.Attack` | Attack montage (slot) |
| `Ability.Dodge` | Dodge montage (slot) |
| `State.Stunned` | Stun additive (full body) |
| `State.Dead` | Death montage |

---

## 💾 Data Persistence

### Save Game Structure

```cpp
// DASaveGame.h
UCLASS()
class UDASaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // Player Identity
    FString PlayerName;
    FGuid PlayerId;
    int32 SaveVersion = 1;
    FDateTime SaveTime;

    // Progression
    int32 Level = 1;
    float CurrentXP = 0;
    float TotalXP = 0;
    TArray<FName> UnlockedTalents;
    TMap<FName, int32> TalentRanks;

    // Variant Progress (Per Variant)
    struct FVariantSaveData
    {
        FName VariantName;
        int32 VariantLevel = 1;
        float VariantXP = 0;
        TArray<FName> UnlockedAbilities;
        TMap<FName, int32> AbilityRanks;
        FTransform LastCheckpoint;
        TArray<FName> CompletedObjectives;
    };
    TMap<FName, FVariantSaveData> VariantData;

    // Inventory
    struct FItemSaveData
    {
        FName ItemId;
        int32 Quantity = 1;
        int32 Level = 1;
        TMap<FName, float> Stats;
    };
    TArray<FItemSaveData> Inventory;
    TArray<FItemSaveData> EquippedItems;

    // World State
    FName CurrentLevel;
    FTransform PlayerTransform;
    TArray<FName> WorldStates;  // Doors opened, bosses killed, etc.
    TMap<FName, float> WorldFloats;  // Puzzle states, etc.
};
```

### Save/Load Flow

```mermaid
sequenceDiagram
    participant Player
    participant UI as Pause Menu
    participant SaveSys as UDASaveSubsystem
    participant Slot as Save Slot
    participant Async as Async Task

    Player->>UI: Press Save
    UI->>SaveSys: RequestSave(SlotName)
    SaveSys->>SaveSys: Gather Save Data
    SaveSys->>Async: AsyncSaveGameToSlot()
    Async->>Slot: Write Binary
    Slot-->>Async: Complete
    Async-->>SaveSys: OnSaveComplete
    SaveSys-->>UI: Success/Failed
    UI-->>Player: Notification
```

---

## 📊 Data Tables & Curves

### XP Curve (Data Table)

| Level | XP Required | Total XP | Talent Points |
|-------|-------------|----------|---------------|
| 1 | 0 | 0 | 0 |
| 2 | 1000 | 1000 | 1 |
| 3 | 2500 | 3500 | 1 |
| 4 | 5000 | 8500 | 1 |
| 5 | 9000 | 17500 | 2 |
| 10 | 50000 | ~200k | 10 |
| 20 | 200000 | ~1.5M | 20 |
| 30 | 500000 | ~4M | 30 |
| 40 | 1000000 | ~9M | 40 |
| 50 | 2000000 | ~20M | 50 |

### Talent Tree Data (JSON)

```json
{
  "TalentTree_ThirdPerson": {
    "Name": "Wayfarer's Path",
    "Categories": [
      {
        "Name": "Exploration",
        "Icon": "UI_Icons.Exploration",
        "Talents": [
          {
            "Id": "Talent_SprintMastery",
            "Name": "Sprint Mastery",
            "Description": "Increase sprint speed by 10% per rank.",
            "MaxRank": 3,
            "Cost": [1, 1, 2],
            "Effect": { "Type": "MoveSpeed", "Op": "Multiply", "Value": [1.05, 1.1, 1.15] },
            "Position": { "X": 0, "Y": 0 },
            "Connections": ["Talent_EndlessStamina"]
          }
        ]
      }
    ]
  }
}
```

---

## 🔗 Related Documents

| Document | Link |
|----------|------|
| **TDD Overview** | [00-Overview.md](00-Overview.md) |
| **Architecture** | [01-Architecture.md](01-Architecture.md) |
| **Module Design** | [02-Module-Design.md](02-Module-Design.md) |
| **Networking** | [04-Networking.md](04-Networking.md) |
| **Performance** | [05-Performance.md](05-Performance.md) |
| **GDD Core Gameplay Loop** | [../GDD/01-Core-Gameplay-Loop.md](../GDD/01-Core-Gameplay-Loop.md) |
| **GDD Combat Systems** | [../GDD/04-Combat-Systems.md](../GDD/04-Combat-Systems.md) |

---

> **Data Ownership**: Input → Character → AbilitySystem → Attributes → StateTree → Animation → Render. Each system owns its data; cross-system via events/tags. No direct attribute mutation outside GAS.