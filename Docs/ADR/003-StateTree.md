# ADR-003: StateTree over Behavior Trees

## Status
✅ Accepted

## Context
Unreal Engine provides two AI/behavior systems: traditional Behavior Trees (BT) and the newer StateTree (introduced in UE 5.0, production-ready in 5.1+). The project needs a flexible, data-driven behavior system for AI and gameplay logic across variants.

## Decision
Use StateTree as the primary behavior framework. Use Behavior Trees only if StateTree cannot express a specific requirement.

## Consequences

### Positive
- **Data-driven**: Schema-based, external data assets, designer-friendly
- **Performance**: Evaluated in batches, SIMD-friendly, no virtual calls in hot path
- **Flexibility**: Tasks, Evaluators, Transitions as separate composable units
- **Integration**: Native Gameplay Tags, GAS, Enhanced Input integration
- **Debugging**: StateTree debugger, visual state inspection, transition logging
- **Hierarchical**: State machines, selectors, sequences, parallel, random
- **External Data**: Bind to any UObject, struct, or gameplay data
- **GameplayStateTree**: Specialized for gameplay abilities/tasks

### Negative
- **Maturity**: Newer, fewer community resources, some bugs
- **Learning curve**: Different paradigm from BT (selectors vs evaluators)
- **Tooling**: Editor tools still evolving
- **Migration**: Existing BT knowledge doesn't directly transfer

### Neutral
- **Coexistence**: Can use both in same project (StateTree can call BT tasks)

## Alternatives Considered
- **Behavior Trees**: Mature, well-documented, but rigid, blackboard-based, slower evaluation
- **Custom State Machine**: Full control, but no editor support, no designer access
- **Gameplay Ability Tasks**: Good for abilities, not for general AI behavior

## References
- [StateTree Documentation](https://docs.unrealengine.com/5.8/en-US/statetree-in-unreal-engine/)
- [Gameplay StateTree](https://docs.unrealengine.com/5.8/en-US/gameplay-statetree-in-unreal-engine/)
- `Content/Variant_*/Blueprints/AI/ST_*.uasset`: StateTrees per variant
- `Source/DACharacters/Private/DAPlayerCharacter.cpp`: StateTreeComponent usage