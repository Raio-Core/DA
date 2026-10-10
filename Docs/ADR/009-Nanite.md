# ADR-009: Nanite for Static Geometry

## Status
🟡 Proposed

## Context
Unreal Engine 5's Nanite virtualized geometry system allows rendering millions of triangles with automatic LOD. The project has extensive static environment geometry (levels, props, architecture).

## Decision
Enable Nanite for all static meshes in environment/level content. Use traditional LODs for skeletal meshes and foliage.

## Consequences

### Positive
- **No LOD Authoring**: Automatic continuous LOD
- **Draw Call Reduction**: Cluster culling, fewer draw calls
- **Memory**: Streaming, only visible detail in memory
- **Quality**: Pixel-perfect detail at all distances
- **Workflow**: Import high-poly → Done (no LOD pipeline)
- **Performance**: GPU-driven culling, efficient on modern GPUs

### Negative
- **Hardware**: Requires DX12/Vulkan, SM6.6+ (GTX 10-series+, RX 5000+)
- **Features**: No vertex animation, no tessellation, limited material complexity
- **Foliage**: Not suitable for vegetation (use traditional + impostors)
- **Skeletal**: Not for animated meshes (characters, creatures)
- **Build Time**: Nanite build adds to cook time
- **Disk Size**: Nanite data larger than traditional LODs

### Neutral
- **Hybrid Approach**: Nanite for architecture/props, traditional for characters/foliage

## Implementation

```ini
; Config/DefaultEngine.ini
[/Script/Engine.NaniteSettings]
r.Nanite=1
r.Nanite.MaxPixelsPerEdge=2.0
r.Nanite.ProxyTrianglePercent=10
r.Nanite.Streaming.MaxResources=2048
r.Nanite.Streaming.MaxStreamingRequests=128

; Per-platform
[/Script/WindowsTargetPlatform.WindowsTargetSettings]
bEnableNanite=True

[/Script/LinuxTargetPlatform.LinuxTargetSettings]
bEnableNanite=True

; Disable for mobile/low-end
[/Script/IOSRuntimeSettings.IOSRuntimeSettings]
bEnableNanite=False
```

## Asset Pipeline

```
Source Art (ZBrush/Blender, 1M+ tris)
    ↓
Export FBX/OBJ (No LODs needed)
    ↓
Import to UE (Enable Nanite checkbox)
    ↓
Nanite Build (Automatic during cook)
    ↓
Runtime: Cluster Culling → Visible Clusters → Draw
```

## Fallback for Non-Nanite Hardware

```cpp
// In GameInstance or Settings
void UDA_GameInstance::ApplyNaniteSettings()
{
    if (!IsNaniteSupported())
    {
        // Fallback: Traditional LODs
        // Ensure all Nanite meshes have fallback LODs
        // Or swap to non-Nanite asset variants
        LoadNonNaniteAssetRegistry();
    }
}
```

## Alternatives Considered
- **Traditional LODs Only**: Full control, but manual authoring, more draw calls
- **Mesh Shaders (Custom)**: Too low-level, reinventing Nanite
- **Imposters for Everything**: Low quality, not for hero geometry

## References
- [Nanite Documentation](https://docs.unrealengine.com/5.8/en-US/nanite-virtualized-geometry/)
- `Content/LevelPrototyping/Meshes/`: Enable Nanite on import
- `Content/Variant_*/Meshes/`: Enable Nanite
- `Config/DefaultEngine.ini`: Nanite settings