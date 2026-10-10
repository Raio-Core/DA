# ADR-002: Enhanced Input over Legacy Input

## Status
✅ Accepted

## Context
Unreal Engine has two input systems: the legacy `InputComponent` system (deprecated) and the new `Enhanced Input` plugin (introduced in UE 5.0, default in 5.1+). The project needs a modern, flexible input system supporting multiple variants.

## Decision
Use Enhanced Input exclusively. Do not use legacy input bindings.

## Consequences

### Positive
- **Mapping Contexts**: Swap entire input schemes per variant (TP, Combat, Platforming, Side-Scroll)
- **Triggers**: Built-in Started/Ongoing/Completed/Triggered + custom (Hold, Tap, DoubleTap, Swipe, Charge)
- **Modifiers**: Dead zone, scaling, sensitivity, axis processing per action
- **Input Actions**: Data-driven, reusable across contexts
- **Runtime remapping**: Full key rebind support without code changes
- **Multi-device**: Keyboard, mouse, gamepad, touch, VR unified
- **Debugging**: Input debugger, visualization tools
- **Future-proof**: Epic's recommended direction

### Negative
- **Learning curve**: Different mental model from legacy
- **Migration**: Existing legacy code must be rewritten
- **Complexity**: More concepts (Context, Action, Trigger, Modifier)
- **Blueprint exposure**: Some advanced features C++ only

### Neutral
- **Legacy compatibility**: Can run both during transition (not recommended)

## Alternatives Considered
- **Legacy Input**: Simple, familiar, but deprecated and lacks variant support
- **Custom Input System**: Full control, but massive effort, no editor tools

## References
- [Enhanced Input Documentation](https://docs.unrealengine.com/5.8/en-US/enhanced-input-in-unreal-engine/)
- `DAPlayerCharacter.cpp`: Input binding implementation
- `Config/Variants/*.json`: Per-variant InputMappingContext references
- `Content/Input/IMC_*.uasset`: Input Mapping Contexts per variant