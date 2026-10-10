# ADR-010: Automation Testing + Gauntlet for Gameplay

## Status
🟡 Proposed

## Context
The project needs a testing strategy covering unit tests (C++), integration tests (editor), and gameplay tests (functional). Unreal provides Automation System (unit/integration) and Gauntlet (gameplay/automation).

## Decision
Use a three-tier testing strategy:
1. **Unit Tests**: C++ Automation Tests (fast, CI)
2. **Integration Tests**: Editor Automation Tests (medium, CI)
3. **Gameplay Tests**: Gauntlet (slow, nightly/release)

## Consequences

### Positive
- **Fast Feedback**: Unit tests run in < 1 min on CI
- **Coverage**: C++ logic, component behavior, system integration
- **Gameplay Validation**: Gauntlet runs actual gameplay scenarios
- **Regression Prevention**: CI blocks merges on test failure
- **Performance Baselines**: Gauntlet captures perf metrics

### Negative
- **Gauntlet Complexity**: Setup, scripting, flakiness
- **Maintenance**: Tests break with design changes
- **CI Time**: Gauntlet adds 10-30 min to pipeline
- **False Negatives**: Environmental issues (GPU, timing)

### Neutral
- **Manual Testing**: Still needed for feel, visuals, UX

## Test Categories

### 1. Unit Tests (C++ Automation)

```cpp
// Source/DA/Tests/DAAttributeSetTest.cpp
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDAAttributeSetTest, "DA.AttributeSet.BaseAttributes",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FDAAttributeSetTest::RunTest(const FString& Parameters)
{
    UDAAttributeSet* AttrSet = NewObject<UDAAttributeSet>();
    
    TestTrue("Health initialized", AttrSet->GetHealth() == 100.0f);
    TestTrue("MaxHealth initialized", AttrSet->GetMaxHealth() == 100.0f);
    TestTrue("Stamina initialized", AttrSet->GetStamina() == 100.0f);
    
    // Test modifier stack
    AttrSet->SetHealth(50.0f);
    TestTrue("Health set", AttrSet->GetHealth() == 50.0f);
    
    return true;
}
```

**Run**: `UnrealEditor-Cmd -run=AutomationTest -TestNames="DA.*"`

### 2. Integration Tests (Editor Automation)

```cpp
// Source/DACharacters/Tests/DAPlayerCharacterTest.cpp
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FDAPlayerCharacterTest, "DACharacters.PlayerCharacter.Movement",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDAPlayerCharacterTest::RunTest(const FString& Parameters)
{
    // Spawn test level
    UWorld* World = GetTestWorld();
    ADAPlayerCharacter* Character = World->SpawnActor<ADAPlayerCharacter>();
    
    // Test movement input
    FInputActionValue MoveValue(FVector2D(0.0f, 1.0f)); // Forward
    Character->Move(MoveValue);
    
    // Tick
    World->Tick(LEVELTICK_All, 0.016f);
    
    // Verify velocity
    FVector Vel = Character->GetVelocity();
    TestTrue("Moving forward", Vel.Y > 0.0f);
    TestTrue("Speed correct", FMath::IsNearlyEqual(Vel.Size(), 600.0f, 10.0f));
    
    return true;
}
```

### 3. Gameplay Tests (Gauntlet)

```csharp
// Gauntlet/Scripts/DACombatTest.cs
public class DACombatTest : GauntletTest
{
    public override void RunTest()
    {
        // Launch game
        LaunchGame("-map=/Game/Variant_Combat/Lvl_Combat");
        
        // Wait for load
        WaitForLevelLoad("Lvl_Combat", 30);
        
        // Spawn player
        var Player = SpawnPlayer("BP_CombatCharacter");
        
        // Test: Basic combat loop
        for (int i = 0; i < 5; i++)
        {
            // Move to enemy
            MoveTo(Player, "Enemy_01");
            
            // Attack
            PressKey(Player, "LeftMouseButton", 0.5f);
            Wait(1.0f);
            
            // Verify damage
            Assert.HealthReduced("Enemy_01", 15.0f, 5.0f);
        }
        
        // Test: Dodge
        PressKey(Player, "SpaceBar"); // Jump/Dodge
        Wait(0.5f);
        Assert.State(Player, "Dodging");
        
        // Capture metrics
        CapturePerfStats("Combat_5Enemy");
        CaptureScreenshot("Combat_Finish");
    }
}
```

## CI Integration

```yaml
# .github/workflows/ci.yml (excerpt)
jobs:
  test:
    runs-on: windows-latest
    steps:
      - name: Run Unit Tests
        run: |
          UnrealEditor-Cmd.exe DA.uproject -run=AutomationTest -TestNames="DA.*" -nullrhi -nosound -unattended
      
      - name: Run Integration Tests
        run: |
          UnrealEditor-Cmd.exe DA.uproject -run=AutomationTest -TestNames="DACharacters.*" -nullrhi -nosound -unattended
      
      - name: Run Gauntlet Tests
        if: github.event_name == 'schedule' || github.ref == 'refs/heads/master'
        run: |
          Gauntlet.exe -script=DACombatTest -build=DAEditor -platform=Win64 -configuration=Development
```

## Alternatives Considered
- **Only Unit Tests**: Fast but misses gameplay bugs
- **Only Gauntlet**: Comprehensive but too slow for CI
- **Third-party (Catch2, GoogleTest)**: Not integrated with UE reflection/GC
- **Blueprint Tests**: Limited to BP logic, no C++ coverage

## References
- [UE Automation Testing](https://docs.unrealengine.com/5.8/en-US/automation-system-in-unreal-engine/)
- [Gauntlet Documentation](https://docs.unrealengine.com/5.8/en-US/gauntlet-automation-framework/)
- `Source/DA/Tests/`, `Source/DACharacters/Tests/`
- `Gauntlet/Scripts/`
- `.github/workflows/ci.yml`: Test job configuration