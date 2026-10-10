# ADR-006: Three-Module Architecture (DA, DACharacters, DAEditor)

## Status
✅ Accepted

## Context
The project needs clear separation between core systems, character implementation, and editor tools. Monolithic module creates coupling, slow compile times, and unclear ownership.

## Decision
Split into three modules with strict dependency direction:

```
DA (Core) ← DACharacters (Implementation) ← DAEditor (Tools)
```

## Consequences

### Positive
- **Compile Times**: Changes to DACharacters don't recompile DAEditor; DA changes only recompile dependents
- **Ownership**: Clear team/module boundaries (Systems vs Characters vs Tools)
- **Reusability**: DA module usable in other projects without character/editor code
- **Testing**: Unit test DA systems in isolation
- **Distribution**: Ship DA + DACharacters as runtime plugin; DAEditor as editor plugin

### Negative
- **Cross-Module Refactoring**: Moving code between modules requires build config changes
- **Circular Dependency Risk**: Must enforce direction in code reviews
- **Shared Types**: Common types (enums, structs) must live in DA (lowest common denominator)

### Neutral
- **Build.cs Complexity**: Three build files instead of one

## Module Responsibilities

| Module | Responsibility | Public API |
|--------|----------------|------------|
| **DA** | Core systems, interfaces, GAS integration, subsystems, GameMode base | AttributeSet, CombatInterface, InputConfig, VariantSubsystem, ProgressionSubsystem, GameMode, PlayerController |
| **DACharacters** | Character implementation, movement, camera, animation, combat components | CharacterBase, PlayerCharacter, MovementComponent, CameraComponent, AnimInstance |
| **DAEditor** | Editor tools, customizations, visualizers, debugging | VariantConfigEditor, InputMappingVisualizer, StateTreeDebugger |

## Dependency Rules

```
DA:          No project dependencies (Engine + Plugins only)
DACharacters: Depends on DA (Public + Private)
DAEditor:    Depends on DA + DACharacters (Public only, Editor-only)
```

## Alternatives Considered
- **Single Module**: Simple, but 10k+ lines, slow compiles, unclear ownership
- **Many Modules**: Per-system (Input, Camera, Movement, Combat, AI, etc.) — too granular, dependency hell
- **Two Modules (Runtime + Editor)**: Better, but character code mixes with core systems

## References
- `Source/DA/DA.Build.cs`
- `Source/DACharacters/DACharacters.Build.cs`
- `Source/DAEditor/DAEditor.Build.cs`
- `Source/DA.Target.cs`, `DAEditor.Target.cs`