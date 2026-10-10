# 🎮 Game Design Document — Variant Designs

<div align="center">

![Version](https://img.shields.io/badge/Version-0.1.0-blue?style=for-the-badge&logo=git)
![Status](https://img.shields.io/badge/Status-In%20Development-orange?style=for-the-badge&logo=trello)

[← Core Gameplay Loop](01-Core-Gameplay-Loop.md) | [Character Design →](03-Character-Design.md)

</div>

---

## 🎯 Variant Philosophy

Each variant is a **distinct gameplay experience** built on the same foundation. They share:
- Core character controller (`ADACharacterBase` → `ADAPlayerCharacter`)
- Enhanced Input system (different `InputMappingContext` per variant)
- Camera system (configured per variant)
- StateTree behavior framework
- Progression hooks

But diverge in:
- **Movement parameters** (speed, jump, air control)
- **Camera behavior** (distance, FOV, constraints)
- **Ability sets** (different `GameplayAbility` grants)
- **StateTree logic** (variant-specific `.uasset` files)
- **Level design patterns** (different layout languages)

---

## 🎮 Variant 1: Third-Person (TP)

### Core Fantasy
> **"Fluid exploration in a living world"** — Seamless traversal, environmental storytelling, emergent encounters.

### Reference Games
| Game | What We Borrow |
|------|----------------|
| *Ghost of Tsushima* | Wind guidance, stance switching, exploration flow |
| *Assassin's Creed Valhalla* | Parkour fluidity, world density, side content |
| *The Witcher 3* | Quest structure, witcher senses, combat preparation |

### Key Mechanics

| System | Implementation |
|--------|----------------|
| **Movement** | Walk/Run/Sprint, Jump, Crouch, Vault, Ledge Grab, Swim |
| **Camera** | Orbit (RMB), Shoulder swap (V), Photo mode, Auto-follow |
| **Traversal** | Climb markers, zip lines, fast travel, mount (future) |
| **Combat** | Light/Heavy/Parry/Dodge, 4 Stances, Ability Wheel (L1/R1) |
| **Perception** | Focus mode (R3), Track highlights, Audio cues |
| **Progression** | Skill trees (3 branches), Gear sets, Legendary weapons |

### Input Mapping (IMC_ThirdPerson)

| Action | Key | Type | Notes |
|--------|-----|------|-------|
| Move | L-Stick / WASD | Axis2D | |
| Look | R-Stick / Mouse | Axis2D | |
| Sprint | L3 / Shift | Digital | Hold |
| Jump | A / Space | Digital | Tap |
| Crouch | B / Ctrl | Digital | Toggle |
| Dodge | Double-tap Move | Digital | Directional |
| Light Attack | RB / LMB | Digital | Tap |
| Heavy Attack | RT / RMB | Digital | Hold |
| Parry | LB / Q | Digital | Timing window |
| Ability 1-4 | D-Pad / 1-4 | Digital | Radial menu |
| Focus | R3 / V | Digital | Toggle |
| Interact | X / E | Digital | Contextual |
| Camera Toggle | View / C | Digital | 1st↔3rd |

### Camera Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Default Distance** | 400cm | Spring arm length |
| **Min Distance** | 150cm | Collision pull-in |
| **Max Distance** | 600cm | Zoom out |
| **FOV** | 90° | Adjustable 70-110 |
| **Pitch Limits** | -80° to +80° | Prevent flipping |
| **Lag Speed** | 10.0 | Smooth follow |
| **Collision Probe** | 12cm radius | Environment avoidance |

### Level Design Language

| Element | Description | Metrics |
|---------|-------------|---------|
| **Main Path** | Critical path, gated by abilities | 3-5 min segments |
| **Side Paths** | Optional, reward exploration | 30-60 sec detours |
| **Verticality** | 3-5 layers (ground, roof, cliff, canopy) | 500-2000cm height diff |
| **Traversal Beats** | Jump, climb, swing, slide every 15-20s | Rhythm |
| **Vistas** | Landmark reveals, photo ops | Every 2-3 min |
| **Encounter Spaces** | 30m diameter, cover, vertical options | Combat arenas |

---

## ⚔️ Variant 2: Combat (CBT)

### Core Fantasy
> **"Mastery through combat depth"** — Precision, timing, adaptation, stylish execution.

### Reference Games
| Game | What We Borrow |
|------|----------------|
| *Devil May Cry 5* | Style meter, combo variety, cancel windows |
| *God of War (2018)* | Weighty attacks, runic abilities, Spartan Rage |
| *Elden Ring* | Stance breaking, guard counters, spirit ashes |
| *Sekiro* | Posture management, deflect, mikiri counter |

### Key Mechanics

| System | Implementation |
|--------|----------------|
| **Stance System** | 4 stances (Speed, Power, Range, Defense), swap instant |
| **Combo System** | Dial-a-combo + dynamic chains, launchers, enders |
| **Posture/Poise** | Enemy posture bar, player poise, break = crit state |
| **Resource** | Focus (blue) for abilities, Adrenaline (yellow) for ultimates |
| **Abilities** | 8 slots (4 stance-specific, 4 universal), cooldowns |
| **Ultimate** | Stance-specific super, builds via combat performance |
| **Enemy AI** | StateTree: Patrol → Investigate → Combat → Flank → Retreat |

### Input Mapping (IMC_Combat)

| Action | Key | Type | Notes |
|--------|-----|------|-------|
| Move | L-Stick / WASD | Axis2D | |
| Look | R-Stick / Mouse | Axis2D | |
| Lock-On | L3 / Tab | Digital | Toggle |
| Light Attack | RB / LMB | Digital | Chainable |
| Heavy Attack | RT / RMB | Digital | Chargeable |
| Dodge | A / Space | Digital | Directional, I-frames |
| Parry/Block | LB / Q | Digital | Hold=Block, Tap=Parry |
| Stance Cycle | D-Pad Up/Down | Digital | 4 stances |
| Ability 1-4 | D-Pad L/R/U/D / 1-4 | Digital | Context per stance |
| Ultimate | L3+R3 / R | Digital | Full meter |
| Interact | X / E | Digital | Executions, pickups |

### Combat Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Light Attack Speed** | 0.35s | Cancel window: 0.2s |
| **Heavy Attack Speed** | 0.75s | Charge levels: 3 |
| **Dodge I-Frames** | 0.3s | Startup: 0.05s |
| **Parry Window** | 0.2s | Success: posture damage |
| **Perfect Parry** | 0.05s | Stun + riposte |
| **Combo Window** | 0.6s | Extendable via talents |
| **Launch Height** | 500cm | Aerial combo enabler |
| **Focus Regen** | 10/s | Combat: 20/s |
| **Adrenaline Gain** | 5/hit | 100 = Ultimate |

### Enemy Archetypes

| Archetype | Behavior | Weakness |
|-----------|----------|----------|
| **Grunt** | Swarm, basic attacks | Any damage, execution |
| **Shield** | Block front, vulnerable back | Flank, heavy break, parry |
| **Berserker** | Hyper-armor, charge | Parry, dodge, stagger |
| **Mage** | Ranged, zone control | Close gap, interrupt cast |
| **Boss** | Multi-phase, unique mechanics | Phase-specific weaknesses |

---

## 🏃 Variant 3: Platforming (PLT)

### Core Fantasy
> **"Precision movement as expression"** — Flow, mastery, speed, creativity.

### Reference Games
| Game | What We Borrow |
|------|----------------|
| *Celeste* | Dash, wall jump, coyote time, assist mode |
| *Hollow Knight* | Movement tech, pogo, dash, shade dash |
| *Super Meat Boy* | Instant restart, tight controls, momentum |
| *Mirror's Edge* | Flow, runner vision, momentum preservation |

### Key Mechanics

| System | Implementation |
|--------|----------------|
| **Dash** | 8-directional, 0.2s I-frames, 0.8s CD, preserves momentum |
| **Wall Jump** | Jump off wall, 0.1s coyote, variable height by hold |
| **Wall Run** | Hold jump on wall, 2s max, 800 speed, jump off anytime |
| **Ledge Grab** | Auto-grab near ledge, climb up/drop, shimmy |
| **Ground Pound** | Down+Jump in air, shockwave, breaks floor, 3s CD |
| **Air Dash** | Dash in air (unlock), resets on ground touch |
| **Momentum** | Speed preserved through jumps, slopes, curves |
| **Checkpoints** | Generous, instant respawn, ghost replay |

### Input Mapping (IMC_Platforming)

| Action | Key | Type | Notes |
|--------|-----|------|-------|
| Move | L-Stick / WASD | Axis2D | |
| Look | R-Stick / Mouse | Axis2D | Limited vertical |
| Jump | A / Space | Digital | Tap=short, Hold=full |
| Dash | LB / Shift | Digital | 8-dir, I-frames |
| Wall Jump | Jump at wall | Digital | Auto |
| Wall Run | Hold Jump at wall | Digital | Auto |
| Ground Pound | Down+Jump (air) | Digital | |
| Crouch/Slide | B / Ctrl | Digital | Slide on slope |
| Restart | Back / R | Digital | Instant checkpoint |
| Ghost Toggle | View / G | Digital | Show best run |

### Movement Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Walk Speed** | 200 cm/s | Precise positioning |
| **Run Speed** | 600 cm/s | Accel: 3000 |
| **Jump Velocity** | 800 cm/s | Variable by hold (0.1-0.5s) |
| **Air Control** | 0.5 | High for precision |
| **Coyote Time** | 0.1s | Forgiving ledge jumps |
| **Jump Buffer** | 0.15s | Input before landing |
| **Dash Distance** | 400 cm | 8-directional |
| **Dash Cooldown** | 0.8s | Resets on ground |
| **Wall Slide Speed** | 100 cm/s | Slow descent |
| **Wall Jump Force** | 600/900 cm/s | Horizontal/Vertical |

### Level Design Language

| Element | Description | Metrics |
|---------|-------------|---------|
| **Screen** | Single-screen challenge | 4000x2000 cm |
| **Checkpoint** | Every 2-3 screens | Instant respawn |
| **Rhythm** | Jump → Dash → Wall → Land | 2-3 sec cycles |
| **Optional Paths** | Harder route, better reward | Strawberry/collectible |
| **Hazards** | Spikes, lasers, crushers, wind | Clear visual language |
| **Assist Mode** | Infinite dash, slow motion, invincible | Accessibility |

---

## 🎮 Variant 4: Side-Scrolling (SS)

### Core Fantasy
> **"2.5D action precision"** — Lane-based combat, parallax depth, crisp controls.

### Reference Games
| Game | What We Borrow |
|------|----------------|
| *Dead Cells* | Procedural, weapon variety, mutation, speed |
| *Mark of the Ninja* | Stealth, vision cones, silent takedowns |
| *Ori and the Blind Forest* | Movement fluidity, bash, spirit edge |
| *Castlevania: SotN* | RPG elements, map, familiar, inverted castle |

### Key Mechanics

| System | Implementation |
|--------|----------------|
| **Lane System** | 3 lanes (Back, Mid, Front), L1/R1 to switch |
| **2D Movement** | X-axis only, Y for jump, Z for lane |
| **Camera** | Fixed orthographic, parallax layers, screen shake |
| **Combat** | Melee (X), Ranged (Y), Special (B), Dodge (A) |
| **Weapon Slots** | 2 equipped, quick swap (D-Pad), distinct movesets |
| **Stealth** | Vision cones, noise meter, silent takedown, hide spots |
| **Metroidvania** | Ability gates, backtracking, map reveal, fast travel |

### Input Mapping (IMC_SideScrolling)

| Action | Key | Type | Notes |
|--------|-----|------|-------|
| Move | L-Stick X / A-D | Axis1D | Horizontal only |
| Aim | R-Stick / Mouse | Axis2D | Aim assist optional |
| Jump | A / Space | Digital | |
| Dash/Dodge | B / Shift | Digital | I-frames, direction |
| Attack | X / LMB | Digital | Melee combo |
| Ranged | Y / RMB | Digital | Aim + shoot |
| Special | B / Q | Digital | Weapon skill |
| Lane Up | LB / W | Digital | Back → Mid → Front |
| Lane Down | LT / S | Digital | Front → Mid → Back |
| Interact | X / E | Digital | Doors, pickups |
| Map | View / Tab | Digital | Overlay |
| Weapon Swap | D-Pad / 1-2 | Digital | Quick swap |

### Camera Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| **Projection** | Orthographic | No perspective distortion |
| **Ortho Width** | 4000 cm | Screen width in world units |
| **Follow Speed** | 20.0 | Snappy horizontal |
| **Vertical Offset** | 200 cm | Player slightly below center |
| **Look Ahead** | 500 cm | Camera leads movement |
| **Parallax Layers** | 5 (BG→FG) | Speeds: 0.1, 0.3, 0.5, 0.8, 1.0 |
| **Screen Shake** | Per impact | Amplitude: 50-200 cm |

### Lane System

| Lane | Z-Position | Use Case |
|------|------------|----------|
| **Back** | -600 cm | Background enemies, cover, stealth |
| **Mid** | 0 cm | Main combat, platforming |
| **Front** | +600 cm | Foreground hazards, pickups, finishers |

### Level Design Language

| Element | Description | Metrics |
|---------|-------------|---------|
| **Room** | Single-screen encounter | 4000x3000 cm |
| **Corridor** | Transition, traversal | 2000x3000 cm |
| **Arena** | Multi-wave combat | 6000x4000 cm |
| **Secret** | Hidden room, breakable wall | 1 per 3-5 rooms |
| **Lane Puzzles** | Switch lanes to progress | Timing, pattern |
| **Vertical Slice** | 3-5 rooms per biome | Distinct palette |

---

## 📊 Variant Comparison Matrix

| Feature | Third-Person | Combat | Platforming | Side-Scroll |
|---------|--------------|--------|-------------|-------------|
| **Camera** | Orbit 3D | Orbit 3D (lock-on) | Orbit 3D (limited) | Ortho 2.5D |
| **Movement Dim** | 3D | 3D | 3D | 2D + Lanes |
| **Core Loop** | Explore → Fight | Prepare → Execute | Survey → Execute | Observe → Engage |
| **Progression** | Open skill tree | Stance mastery | Movement tech | Metroidvania |
| **Difficulty** | Accessible | High skill ceiling | Precision | Roguelike |
| **Session Length** | 30-60 min | 15-30 min | 10-20 min | 20-40 min |
| **Replayability** | Builds, exploration | Mastery, scores | Speedrun, % | Runs, builds |
| **Target Audience** | Action-adventure fans | Character action fans | Platformer fans | Roguelike/Metroidvania fans |

---

## 🔧 Technical Implementation

### Variant Activation

```cpp
// In GameMode or VariantManager
UCLASS()
class ADAVariantGameMode : public AGameModeBase
{
    UPROPERTY(EditDefaultsOnly, Category = "Variant")
    TSubclassOf<ADAPlayerCharacter> VariantCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "Variant")
    TSoftObjectPtr<UInputMappingContext> VariantInputContext;

    UPROPERTY(EditDefaultsOnly, Category = "Variant")
    TSoftObjectPtr<UStateTree> VariantStateTree;

    virtual void BeginPlay() override
    {
        // Apply variant-specific defaults
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (ADAPlayerCharacter* Char = Cast<ADAPlayerCharacter>(PC->GetPawn()))
            {
                Char->ApplyVariantConfiguration(VariantCharacterClass, VariantInputContext, VariantStateTree);
            }
        }
    }
};
```

### Data-Driven Variant Config

```json
// Config/Variants/ThirdPerson.json
{
  "VariantName": "ThirdPerson",
  "DisplayName": "Third Person Adventure",
  "CharacterClass": "/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter",
  "InputMappingContext": "/Game/Input/IMC_ThirdPerson",
  "StateTree": "/Game/ThirdPerson/ST_ThirdPerson",
  "CameraConfig": {
    "DefaultDistance": 400,
    "MinDistance": 150,
    "MaxDistance": 600,
    "FOV": 90,
    "PitchLimits": [-80, 80]
  },
  "MovementConfig": {
    "WalkSpeed": 300,
    "RunSpeed": 600,
    "SprintMultiplier": 2.0,
    "JumpVelocity": 700,
    "AirControl": 0.35
  },
  "Progression": {
    "TalentTree": "/Game/Progression/TalentTree_ThirdPerson",
    "StartingAbilities": ["Dash", "Focus", "Vault"]
  }
}
```

---

## 🔗 Related Documents

| Document | Link |
|----------|------|
| **GDD Overview** | [00-Overview.md](00-Overview.md) |
| **Core Gameplay Loop** | [01-Core-Gameplay-Loop.md](01-Core-Gameplay-Loop.md) |
| **Character Design** | [03-Character-Design.md](03-Character-Design.md) |
| **Combat Systems** | [04-Combat-Systems.md](04-Combat-Systems.md) |
| **Level Design** | [05-Level-Design.md](05-Level-Design.md) |
| **TDD Module Design** | [../TDD/02-Module-Design.md](../TDD/02-Module-Design.md) |
| **TDD Data Flow** | [../TDD/03-Data-Flow.md](../TDD/03-Data-Flow.md) |

---

> **Implementation Note**: Variant configs live in `Config/Variants/*.json`. Load via `DAVariantSubsystem` at runtime. Each variant has its own GameMode BP inheriting from `ADAVariantGameMode`.