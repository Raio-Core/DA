# ADR-007: Async Asset Loading for Variants

## Status
🟡 Proposed

## Context
Variant switching currently loads all assets synchronously during `ApplyVariant()`, causing hitches (200-500ms). As variant content grows, this will impact gameplay flow.

## Decision
Implement async loading for variant assets using `TSoftObjectPtr` + `AsyncLoad` + `StreamableManager`. Show loading screen during transition.

## Consequences

### Positive
- **No Hitches**: Loading spread over frames
- **Streaming**: Assets stream in priority order
- **Cancellable**: Can abort if player cancels switch
- **Progress UI**: Show loading progress to player

### Negative
- **Complexity**: Async state machine for variant switch
- **Fallback**: Need placeholder character during load
- **Memory**: Both old and new variant assets in memory during transition
- **Testing**: Race conditions, partial load states

### Neutral
- **Current State**: Synchronous, acceptable for current content size

## Implementation Plan

```cpp
// DAVariantSubsystem.h
UFUNCTION(BlueprintCallable, Category = "DA|Variant")
void SwitchVariantAsync(FName NewVariant, FOnVariantSwitchComplete Callback);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariantSwitchComplete, bool, bSuccess);

// Implementation
void UDAVariantSubsystem::SwitchVariantAsync(FName NewVariant, FOnVariantSwitchComplete Callback)
{
    if (ActiveVariant == NewVariant) { Callback.Execute(true); return; }
    
    // 1. Unload current variant (keep essential)
    UnloadVariantAssets(ActiveVariant);
    
    // 2. Collect new variant assets
    FVariantConfig Config;
    if (!LoadVariantConfig(NewVariant, Config)) { Callback.Execute(false); return; }
    
    TArray<TSoftObjectPtr<UObject>> AssetsToLoad;
    AssetsToLoad.Add(Config.CharacterClass);
    AssetsToLoad.Add(Config.InputContext);
    AssetsToLoad.Add(Config.StateTree);
    AssetsToLoad.Add(Config.CameraConfig);
    AssetsToLoad.Add(Config.MovementConfig);
    
    // 3. Async load
    FStreamableDelegate OnComplete = FStreamableDelegate::CreateUObject(
        this, &UDAVariantSubsystem::OnVariantAssetsLoaded, NewVariant, Callback);
    
    StreamableManager.RequestAsyncLoad(AssetsToLoad, OnComplete, 100); // High priority
}

void UDAVariantSubsystem::OnVariantAssetsLoaded(FName NewVariant, FOnVariantSwitchComplete Callback)
{
    // Apply config now that assets are loaded
    bool bSuccess = ApplyVariantConfigSync(NewVariant);
    Callback.Execute(bSuccess);
}
```

## Alternatives Considered
- **Synchronous with Background Thread**: UE doesn't support true background loading for UObjects
- **Preload All Variants**: Memory heavy, defeats purpose
- **Level Streaming**: Variants are not levels; different approach

## References
- [Async Loading in UE](https://docs.unrealengine.com/5.8/en-US/async-loading-in-unreal-engine/)
- `FStreamableManager`, `TSoftObjectPtr`
- `Source/DA/Private/DAVariantSubsystem.cpp` (future implementation)