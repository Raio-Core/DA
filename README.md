# 🎮 DA — Dynamic Action Framework

<div align="center">

![Unreal Engine](https://img.shields.io/badge/Unreal_Engine-5.8-%230E1128?style=for-the-badge&logo=unrealengine&logoColor=white)
![C++](https://img.shields.io/badge/C++-20-%2300599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Win64-%230078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

[![Build Status](https://img.shields.io/github/actions/workflow/status/Raio-Core/DA/ci.yml?branch=master&style=for-the-badge&logo=githubactions)](https://github.com/Raio-Core/DA/actions)
[![Last Commit](https://img.shields.io/github/last-commit/Raio-Core/DA?style=for-the-badge&logo=git&logoColor=white)](https://github.com/Raio-Core/DA/commits/master)
[![Repo Size](https://img.shields.io/github/repo-size/Raio-Core/DA?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Raio-Core/DA)
[![Languages](https://img.shields.io/github/languages/count/Raio-Core/DA?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Raio-Core/DA/search?l=cpp)

[![Stars](https://img.shields.io/github/stars/Raio-Core/DA?style=social)](https://github.com/Raio-Core/DA/stargazers)
[![Forks](https://img.shields.io/github/forks/Raio-Core/DA?style=social)](https://github.com/Raio-Core/DA/network/members)
[![Issues](https://img.shields.io/github/issues/Raio-Core/DA?style=social)](https://github.com/Raio-Core/DA/issues)
[![Contributors](https://img.shields.io/github/contributors/Raio-Core/DA?style=social)](https://github.com/Raio-Core/DA/graphs/contributors)

</div>

---

## 🌟 Overview

**DA (Dynamic Action)** is a modular, extensible Unreal Engine 5.8 framework for building dynamic gameplay systems. Built with a **plugin-based architecture** supporting multiple gameplay variants (Third-Person, Combat, Platforming, Side-Scrolling) from a single codebase.

```mermaid
graph TB
    subgraph "Core Framework"
        DA[DA Runtime Module]
        DAChars[DACharacters Module]
        DAEditor[DAEditor Module]
    end

    subgraph "Gameplay Variants"
        TP[Third Person]
        CBT[Combat]
        PLT[Platforming]
        SS[Side Scrolling]
    end

    subgraph "Engine Systems"
        EI[Enhanced Input]
        GS[Gameplay StateTree]
        ST[StateTree]
        AI[AI/Navigation]
    end

    DA --> DAChars
    DA --> DAEditor
    DAChars --> TP
    DAChars --> CBT
    DAChars --> PLT
    DAChars --> SS
    DAChars --> EI
    DAChars --> GS
    DAChars --> ST
    CBT --> AI
```

---

## 🏗 Architecture

### Module Dependency Graph

```mermaid
graph LR
    subgraph "Runtime Modules"
        DA[DA<br/>Core Gameplay]
        DAChars[DACharacters<br/>Character System]
    end

    subgraph "Editor Module"
        DAEditor[DAEditor<br/>Editor Utilities]
    end

    subgraph "Engine Plugins"
        EI[EnhancedInput]
        ST[StateTree]
        GS[GameplayStateTree]
        NI[Niagara]
    end

    DA --> EI
    DA --> ST
    DA --> GS
    DAChars --> DA
    DAChars --> EI
    DAChars --> ST
    DAEditor --> DA
    DAEditor --> DAChars
```

### Character Class Hierarchy

```mermaid
classDiagram
    class ACharacter {
        <<Engine>>
        +GetCharacterMovement()
        +GetController()
        +GetMesh()
    }

    class ADACharacterBase {
        +CanCharacterJump() bool
        +HasJumped()
        #BeginPlay()
        #Tick()
        #SetupPlayerInputComponent()
    }

    class ADAPlayerCharacter {
        -CameraBoom: USpringArmComponent
        -ThirdPersonCamera: UCameraComponent
        -FirstPersonCamera: UCameraComponent
        -DefaultMappingContext: UInputMappingContext
        -JumpAction: UInputAction
        -MoveAction: UInputAction
        -LookAction: UInputAction
        -ToggleCameraPerspective: UInputAction
        +Move()
        +Look()
        +PlayerJump()
        +TogglePerspective()
        #BeginPlay()
        #SetupPlayerInputComponent()
    }

    ACharacter <|-- ADACharacterBase
    ADACharacterBase <|-- ADAPlayerCharacter
```

---

## ⚡ Features

| Feature | Status | Module | Description |
|---------|--------|--------|-------------|
| **Multi-Variant Gameplay** | ✅ | DACharacters | Switch between Third-Person, Combat, Platforming, Side-Scrolling |
| **Enhanced Input System** | ✅ | DACharacters | Modern input with mapping contexts, triggers, modifiers |
| **Dual Camera System** | ✅ | DACharacters | Seamless First/Third person toggle with smooth transitions |
| **StateTree AI** | 🚧 | DACharacters | Behavior trees via GameplayStateTree plugin |
| **Modular Ability System** | 📋 | DA | Planned: Gameplay Ability System integration |
| **Procedural Animation** | 📋 | DACharacters | Planned: Motion matching, IK rigs |
| **Network Replication** | 📋 | DA | Planned: Multiplayer-ready architecture |

---

## 🚀 Quick Start

### Prerequisites

| Tool | Version | Required |
|------|---------|----------|
| Unreal Engine | 5.8+ | ✅ |
| Visual Studio | 2022 17.8+ | ✅ |
| Windows SDK | 10.0.22621+ | ✅ |
| .NET SDK | 8.0+ | ✅ (for UBT) |

### Installation

```bash
# Clone the repository
git clone https://github.com/Raio-Core/DA.git
cd DA

# Generate project files (run from engine root or use UE Project Browser)
# Right-click DA.uproject → "Generate Visual Studio project files"

# Open in Unreal Editor
# Double-click DA.uproject
```

### Build Configuration

```mermaid
flowchart TD
    A[Clone Repo] --> B{Generate Project Files}
    B --> C[Open DA.sln in VS2022]
    C --> D[Select Configuration: Development Editor]
    D --> E[Build Solution]
    E --> F[Launch Editor from VS or .uproject]
    F --> G[Play in Editor / Standalone]
```

---

## 📁 Project Structure

```
DA/
├── 📁 Config/                    # Engine & project configuration
│   ├── DefaultEngine.ini         # Engine settings, input, rendering
│   ├── DefaultGame.ini           # Gameplay defaults, maps
│   ├── DefaultInput.ini          # Input bindings (legacy)
│   └── DefaultEditor.ini         # Editor preferences
│
├── 📁 Content/                   # All assets (Blueprints, Maps, Materials)
│   ├── 📁 Characters/            # Character meshes, anims, materials
│   │   └── 📁 Mannequins/        # UE5 Manny/Quinn assets
│   ├── 📁 Input/                 # Enhanced Input assets
│   │   ├── 📁 Actions/           # Input Actions (IA_Jump, IA_Move, etc.)
│   │   ├── IMC_Default.uasset    # Input Mapping Context
│   │   └── IMC_MouseLook.uasset
│   ├── 📁 LevelPrototyping/      # Greyboxing tools
│   ├── 📁 ThirdPerson/           # Third-person variant
│   ├── 📁 Variant_Combat/        # Combat variant (AI, abilities)
│   ├── 📁 Variant_Platforming/   # Platforming variant (dash, moving platforms)
│   └── 📁 Variant_SideScrolling/ # 2.5D side-scroller variant
│
├── 📁 Source/                    # C++ Source Code
│   ├── 📁 DA/                    # Core runtime module
│   │   ├── DA.Build.cs           # Module build config
│   │   ├── DA.h/.cpp             # Module entry point
│   │   ├── DACharacter.h/.cpp    # Base character (DEPRECATED → DACharacterBase)
│   │   ├── DAGameMode.h/.cpp     # Base GameMode
│   │   └── DAPlayerController.h/.cpp
│   │
│   ├── 📁 DACharacters/          # Character system module
│   │   ├── DACharacters.Build.cs
│   │   ├── 📁 Public/
│   │   │   ├── DACharacterBase.h     # Base character class
│   │   │   ├── DACharacters.h        # Module public header
│   │   │   └── DAPlayerCharacter.h   # Player character
│   │   └── 📁 Private/
│   │       ├── DACharacters.cpp      # Module implementation
│   │       ├── DACharacterBase.cpp
│   │       └── DAPlayerCharacter.cpp # Enhanced input, movement, camera
│   │
│   ├── DA.Target.cs              # Runtime target
│   └── DAEditor.Target.cs        # Editor target
│
├── 📁 Plugins/                   # Git submodules / local plugins (optional)
│
├── DA.uproject                   # Project descriptor
├── .gitignore                    # Git ignore rules
├── .vsconfig                     # VS component recommendations
└── README.md                     # This file
```

---

## ⌨️ Input System

### Enhanced Input Architecture

```mermaid
sequenceDiagram
    participant Player
    participant InputAction as Input Action (IA_Move)
    participant MappingContext as Input Mapping Context (IMC_Default)
    participant EnhancedInput as Enhanced Input Component
    participant Character as ADAPlayerCharacter

    Player->>InputAction: Presses W (Axis2D Y=1.0)
    InputAction->>MappingContext: Maps to MoveAction
    MappingContext->>EnhancedInput: Triggers (Triggered event)
    EnhancedInput->>Character: BindAction(MoveAction, Triggered, Move)
    Character->>Character: Move(FInputActionValue)
    Character->>Character: Calculate YawRotation from Controller
    Character->>Character: AddMovementInput(Forward, Y)
```

### Default Input Mappings

| Action | Type | Keys | Trigger | Gameplay |
|--------|------|------|---------|----------|
| `IA_Move` | Axis2D | WASD / Left Stick | `Triggered` | Movement |
| `IA_Look` | Axis2D | Mouse / Right Stick | `Triggered` | Camera Rotation |
| `IA_Jump` | Digital | Space / A (Gamepad) | `Started`/`Completed` | Jump |
| `IA_ToggleCameraPerspective` | Digital | V / DPad Up | `Completed` | 1st↔3rd Person |

---

## 🎮 Gameplay Variants

### Variant Comparison

```mermaid
graph TD
    Base[DACharacterBase<br/>Core Movement, Camera, Input]
    
    Base --> TP[ThirdPerson<br/>Standard 3D locomotion]
    Base --> CBT[Combat<br/>Targeting, abilities, AI]
    Base --> PLT[Platforming<br/>Dash, wall-jump, precision]
    Base --> SS[SideScrolling<br/>2.5D, locked camera]

    TP --> TPMap[Lvl_ThirdPerson.umap]
    CBT --> CBTMap[Lvl_Combat.umap]
    PLT --> PLTMap[Lvl_Platforming.umap]
    SS --> SSMap[Lvl_SideScrolling.umap]
```

### Switching Variants

1. **Via GameMode**: Set `DefaultPawnClass` to variant-specific character BP
2. **Via Level**: Each variant has its own map (`Lvl_*.umap`)
3. **Runtime**: Use `GameplayStateTree` to swap behavior trees

---

## 🔧 Development

### Adding a New Gameplay Variant

```mermaid
flowchart LR
    A[Create Variant Folder<br/>Content/Variant_New/] --> B[Create Character BP<br/>Inherit ADAPlayerCharacter]
    B --> C[Create GameMode BP<br/>Inherit ADAGameMode]
    C --> D[Create Input Mapping Context<br/>IMC_New.uasset]
    D --> E[Create Level<br/>Lvl_New.umap]
    E --> F[Register in DA.uproject<br/>Add to PakList if packaging]
```

### Coding Standards

| Aspect | Standard |
|--------|----------|
| **Naming** | PascalCase for classes, camelCase for variables, `b` prefix for bools |
| **Headers** | Module prefix (`DACHARACTERS_API`), `#pragma once` |
| **Includes** | Engine → Plugin → Project → Module (alphabetical within group) |
| **UPROPERTY** | Category grouping, `meta=(AllowPrivateAccess="true")` for private |
| **Logging** | `UE_LOG(LogDA, Verbose/Log/Warning/Error, TEXT("..."))` |

### Useful Console Commands

```ini
; In DefaultEngine.ini or console
LogDA Verbose          ; Enable DA module logging
ShowDebug AbilitySystem; Debug GAS (when integrated)
ShowDebug AI           ; Debug AI/StateTree
p.VisualizeMovement 1  ; Show movement vectors
```

---

## 📦 Packaging

### Build Pipeline

```mermaid
graph LR
    A[Source Control] --> B[CI: GitHub Actions]
    B --> C{Build Config}
    C -->|Win64| D[Development Editor]
    C -->|Win64| E[Shipping Client]
    C -->|Win64| F[Shipping Server]
    D --> G[Artifacts]
    E --> G
    F --> G
    G --> H[Release / Deploy]
```

### GitHub Actions Workflow (`.github/workflows/ci.yml`)

```yaml
name: CI

on:
  push:
    branches: [master, develop]
  pull_request:
    branches: [master]

jobs:
  build:
    runs-on: windows-latest
    timeout-minutes: 60
    
    steps:
    - uses: actions/checkout@v4
      with:
        submodules: recursive
        lfs: true

    - name: Setup .NET
      uses: actions/setup-dotnet@v4
      with:
        dotnet-version: '8.0.x'

    - name: Cache Engine
      uses: actions/cache@v4
      with:
        path: |
          D:\Game Engines\UE_5.8
        key: ue58-${{ runner.os }}

    - name: Generate Project Files
      run: |
        & "D:\Game Engines\UE_5.8\Engine\Build\BatchFiles\GenerateProjectFiles.bat" "D:\Work Station\UE\DA\DA.uproject"

    - name: Build Editor
      run: |
        & "D:\Game Engines\UE_5.8\Engine\Build\BatchFiles\Build.bat" DAEditor Win64 Development -Project="D:\Work Station\UE\DA\DA.uproject" -WaitMutex

    - name: Upload Build Artifacts
      uses: actions/upload-artifact@v4
      with:
        name: DA-Editor-${{ github.sha }}
        path: D:\Work Station\UE\DA\Binaries\Win64\UnrealEditor-DA*.dll
        retention-days: 7
```

---

## 📊 Repository Analytics

### Commit Activity

![Commit Activity](https://github.com/Raio-Core/DA/blob/master/.github/assets/commit-activity.svg?raw=true)

### Language Breakdown

![Languages](https://github.com/Raio-Core/DA/blob/master/.github/assets/languages.svg?raw=true)

### Contributors

![Contributors](https://contrib.rocks/image?repo=Raio-Core/DA&max=20&columns=5)

---

## 🤝 Contributing

```mermaid
flowchart TD
    A[Fork Repository] --> B[Create Feature Branch]
    B --> C[Make Changes]
    C --> D[Run Tests Locally]
    D --> E[Push to Fork]
    E --> F[Open Pull Request]
    F --> G[Code Review]
    G --> H{Approved?}
    H -->|Yes| I[Merge to Master]
    H -->|No| C
    I --> J[Deploy / Release]
```

### Branch Strategy

| Branch | Purpose | Protection |
|--------|---------|------------|
| `master` | Production-ready | ✅ Required reviews, CI passing |
| `develop` | Integration branch | ✅ CI passing |
| `feature/*` | New features | ❌ |
| `bugfix/*` | Bug fixes | ❌ |
| `release/*` | Release prep | ✅ CI passing |

### Commit Convention

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:** `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `chore`, `build`, `ci`

**Example:**
```
feat(input): add sprint toggle action

- Add IA_Sprint input action
- Bind SprintOn/SprintOff in SetupPlayerInputComponent
- Update IMC_Default with sprint mapping

Closes #42
```

---

## 🛣 Roadmap

### v1.0 — Foundation (Current)
- [x] Core module structure
- [x] Character base with dual camera
- [x] Enhanced Input integration
- [x] Four gameplay variants scaffolded
- [x] CI/CD pipeline

### v1.1 — Combat Systems
- [ ] Gameplay Ability System integration
- [ ] Combat StateTree behaviors
- [ ] Targeting & lock-on system
- [ ] Hit reactions & damage numbers

### v1.2 — Advanced Movement
- [ ] Motion matching animation
- [ ] Procedural IK (foot placement, ledge grab)
- [ ] Parkour system (vault, climb, slide)
- [ ] Network prediction & reconciliation

### v1.3 — Editor Tools
- [ ] Variant configuration asset
- [ ] Input mapping visualizer
- [ ] StateTree debugger widget
- [ ] Automated playtest framework

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2026 Raioix & Raiocore

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

---

## 🙏 Acknowledgments

| Resource | Link |
|----------|------|
| Unreal Engine Documentation | [docs.unrealengine.com](https://docs.unrealengine.com) |
| Enhanced Input Plugin | [UE5 Docs](https://docs.unrealengine.com/5.8/en-US/enhanced-input-in-unreal-engine/) |
| StateTree Plugin | [UE5 Docs](https://docs.unrealengine.com/5.8/en-US/statetree-in-unreal-engine/) |
| Gameplay StateTree | [UE5 Docs](https://docs.unrealengine.com/5.8/en-US/gameplay-statetree-in-unreal-engine/) |
| Community Tutorials | [Unreal Community Wiki](https://github.com/Allar/unreal-community-wiki) |

---

## 📞 Contact & Support

<div align="center">

[![GitHub Issues](https://img.shields.io/badge/Issues-GitHub-%23E86C00?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Raio-Core/DA/issues)
[![Discussions](https://img.shields.io/badge/Discussions-GitHub-%238E44AD?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Raio-Core/DA/discussions)
[![Discord](https://img.shields.io/badge/Discord-Community-%235865F2?style=for-the-badge&logo=discord&logoColor=white)](https://discord.gg/unrealengine)

**Maintainers:** [@Raioix](https://github.com/Raioix) • [@Raiocore](https://github.com/Raiocore)

</div>

---

## 👥 Contributors

<!-- CONTRIBUTORS_START -->
Thanks to all contributors who have helped make DA Framework better!

[![Contributors](https://contrib.rocks/image?repo=Raio-Core/DA&max=20&columns=5)](https://github.com/Raio-Core/DA/graphs/contributors)
<!-- CONTRIBUTORS_END -->

---

<div align="center">
<sub>Built with ❤️ using Unreal Engine 5.8 • Last updated: <strong id="last-updated">Loading...</strong></sub>
</div>

<script>
  // Auto-update last commit date
  fetch('https://api.github.com/repos/Raio-Core/DA/commits/master')
    .then(r => r.json())
    .then(data => {
      const date = new Date(data.commit.author.date);
      document.getElementById('last-updated').textContent = date.toLocaleDateString('en-US', {
        year: 'numeric', month: 'long', day: 'numeric'
      });
    })
    .catch(() => {
      document.getElementById('last-updated').textContent = 'Unknown';
    });
</script>

---

> **Note:** This README uses GitHub-flavored Markdown with Mermaid.js diagrams. Diagrams render natively on GitHub. For local viewing, use a Markdown viewer with Mermaid support (VS Code + Markdown Preview Mermaid, Obsidian, etc.).