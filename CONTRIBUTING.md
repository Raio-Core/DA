# Contributing to DA Framework

Thank you for your interest in contributing! This document outlines the process and standards for contributing to the DA Framework.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Documentation](#documentation)

## 🤝 Code of Conduct

This project adheres to the [Contributor Covenant Code of Conduct](https://www.contributor-covenant.org/version/2/1/code_of_conduct/). By participating, you are expected to uphold this code.

## 🚀 Getting Started

### Prerequisites

- **Unreal Engine 5.8+** (installed via Epic Games Launcher)
- **Visual Studio 2022 17.8+** with "Game development with C++" workload
- **Windows 10/11 SDK (10.0.22621+)**
- **Git** with LFS support (`git lfs install`)
- **.NET 8.0 SDK** (for UnrealBuildTool)

### First-Time Setup

```bash
# 1. Fork the repository on GitHub
# 2. Clone your fork
git clone https://github.com/YOUR_USERNAME/DA.git
cd DA

# 3. Add upstream remote
git remote add upstream https://github.com/Raio-Core/DA.git

# 4. Install Git LFS
git lfs install
git lfs pull

# 5. Generate project files
# Right-click DA.uproject → "Generate Visual Studio project files"
# OR run from command line:
& "D:\Game Engines\UE_5.8\Engine\Build\BatchFiles\GenerateProjectFiles.bat" DA.uproject

# 6. Open DA.sln in Visual Studio 2022
# 7. Set configuration to "Development Editor" and platform "Win64"
# 8. Build Solution (Ctrl+Shift+B)
```

## 🔄 Development Workflow

### Branch Strategy

```
master (protected)
  ↑
  │  release/v1.0 (protected)
  │  ↑
  │  │  develop (protected)
  │  │  ↑
  │  │  ├── feature/new-ability-system
  │  │  ├── feature/motion-matching
  │  │  ├── bugfix/camera-jitter
  │  │  └── bugfix/input-lag
```

| Branch | Purpose | Naming |
|--------|---------|--------|
| `master` | Production releases | `master` |
| `develop` | Integration branch | `develop` |
| `release/*` | Release preparation | `release/vX.Y` |
| `feature/*` | New features | `feature/short-description` |
| `bugfix/*` | Bug fixes | `bugfix/short-description` |
| `hotfix/*` | Critical production fixes | `hotfix/short-description` |

### Workflow Steps

1. **Sync with upstream**
   ```bash
   git checkout develop
   git pull upstream develop
   ```

2. **Create feature branch**
   ```bash
   git checkout -b feature/awesome-feature
   ```

3. **Make changes** (see Coding Standards below)

4. **Test locally**
   ```bash
   # Build in VS
   # Run editor tests
   # Run gameplay tests if applicable
   ```

5. **Commit with conventional messages**
   ```bash
   git add .
   git commit -m "feat(combat): add combo attack system"
   ```

6. **Push to your fork**
   ```bash
   git push origin feature/awesome-feature
   ```

7. **Open Pull Request** against `develop` branch

## 📝 Coding Standards

### C++ Style Guide

#### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| Classes | PascalCase + Module prefix | `ADAPlayerCharacter` |
| Interfaces | PascalCase + `I` prefix | `IDamageable` |
| Structs | PascalCase + `F` prefix | `FInputActionValue` |
| Enums | PascalCase + `E` prefix | `ETriggerEvent` |
| Functions | PascalCase | `SetupPlayerInputComponent()` |
| Variables (member) | camelCase + `m_` prefix | `m_CurrentHealth` |
| Variables (local) | camelCase | `currentHealth` |
| Booleans | camelCase + `b` prefix | `bIsDead` |
| Constants | UPPER_SNAKE_CASE | `MAX_HEALTH` |
| Macros | UPPER_SNAKE_CASE | `DA_LOG()` |

#### File Organization

```cpp
// 1. Copyright header
// Copyright Raioix & Raiocore. All Rights Reserved 2026.

// 2. Module API macro
#include "DACharacterBase.h"

// 3. Engine includes (alphabetical)
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// 4. Plugin includes
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// 5. Project includes (alphabetical)
#include "InputActionValue.h"

// 6. Implementation
ADAPlayerCharacter::ADAPlayerCharacter()
{
    // Constructor body
}
```

#### UPROPERTY Guidelines

```cpp
// GOOD: Categorized, documented, proper metadata
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Abilities", 
          meta = (AllowPrivateAccess = "true", ClampMin = "0.0", UIMin = "0.0"))
float BaseDamage = 25.0f;

// GOOD: Component references
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", 
          meta = (AllowPrivateAccess = "true"))
class USpringArmComponent* CameraBoom;

// AVOID: Public member variables without UPROPERTY
// float Health; // Bad - no replication, no editor exposure
```

### Header File Template

```cpp
// Copyright Raioix & Raiocore. All Rights Reserved 2026.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyNewClass.generated.h"

class UMyComponent;
struct FMyStruct;

UCLASS(Blueprintable, BlueprintType, Category = "DA|Gameplay")
class DACHARACTERS_API AMyNewClass : public ACharacter
{
    GENERATED_BODY()

public:
    // Constructor
    AMyNewClass();

    // Interface
    virtual void MyPublicFunction() override;

protected:
    // Lifecycle
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Internal helpers
    void InternalHelperFunction();

private:
    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
              meta = (AllowPrivateAccess = "true"))
    UMyComponent* MyComponent;

    // Config
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config",
              meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float ConfigValue = 100.0f;
};
```

### Blueprint Standards

- **Naming**: `BP_` prefix for Blueprints, `WBP_` for Widget Blueprints, `ABP_` for Animation Blueprints
- **Organization**: Use folders matching C++ module structure
- **Inheritance**: Prefer C++ base classes, extend in Blueprint
- **Events**: Use `Event Graph` for initialization, `Function Graphs` for logic
- **Variables**: Categorize with `|` separator (e.g., `Combat|Weapons|CurrentAmmo`)

## 📦 Commit Guidelines

We follow [Conventional Commits](https://www.conventionalcommits.org/) specification.

### Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

| Type | Description |
|------|-------------|
| `feat` | New feature |
| `fix` | Bug fix |
| `docs` | Documentation only |
| `style` | Formatting, missing semicolons, etc. |
| `refactor` | Code restructuring without behavior change |
| `perf` | Performance improvement |
| `test` | Adding/updating tests |
| `chore` | Maintenance, build config, dependencies |
| `build` | Build system changes |
| `ci` | CI/CD configuration |
| `revert` | Reverting previous commit |

### Scopes

| Scope | Area |
|-------|------|
| `core` | DA module (DACharacter, DAGameMode, etc.) |
| `chars` | DACharacters module (DAPlayerCharacter, DACharacterBase) |
| `input` | Enhanced Input system |
| `camera` | Camera system |
| `combat` | Combat variant |
| `platform` | Platforming variant |
| `sidescroll` | Side-scrolling variant |
| `anim` | Animation/Animation Blueprint |
| `ai` | AI/StateTree |
| `build` | Build configuration |
| `docs` | Documentation |

### Examples

```bash
# Feature
git commit -m "feat(chars): add first-person camera toggle

- Add TogglePerspective() to ADAPlayerCharacter
- Bind IA_ToggleCameraPerspective in SetupPlayerInputComponent
- Smooth transition with 0.25s blend

Closes #15"

# Bug fix
git commit -m "fix(input): resolve movement axis inversion

- Swap X/Y axes in Move() to match Enhanced Input convention
- X now maps to strafe (RightDirection)
- Y now maps to forward/back (ForwardDirection)

Fixes #42"

# Refactor
git commit -m "refactor(core): extract DACharacterBase from DACharacter

- Move common functionality to ADACharacterBase
- ADAPlayerCharacter now inherits from DACharacterBase
- Deprecate ADACharacter class

Refs #8"
```

### Footer

- **Breaking Changes**: `BREAKING CHANGE: <description>`
- **Issue References**: `Closes #123`, `Fixes #456`, `Refs #789`
- **Co-authors**: `Co-authored-by: Name <email@domain.com>`

## 🔀 Pull Request Process

### PR Requirements

- [ ] **Target branch**: `develop` (or `release/*` for release fixes)
- [ ] **Title**: Follows conventional commit format
- [ ] **Description**: Clear explanation of changes
- [ ] **Linked Issues**: Reference related issues
- [ ] **Tests**: New/updated tests for changes
- [ ] **Documentation**: Updated if API changed
- [ ] **CI Passing**: All checks green
- [ ] **Review**: At least 1 approval from maintainer

### PR Template

```markdown
## Description
Brief summary of changes

## Type
- [ ] Feature
- [ ] Bug Fix
- [ ] Refactor
- [ ] Documentation
- [ ] Performance
- [ ] Test

## Changes
- Change 1
- Change 2

## Testing
- [ ] Builds in Development Editor
- [ ] Builds in Shipping
- [ ] Manual testing done
- [ ] Automated tests pass

## Screenshots/Videos
(If applicable)

## Related Issues
Closes #XXX
Refs #YYY

## Checklist
- [ ] Code follows style guide
- [ ] Self-review completed
- [ ] Comments added for complex logic
- [ ] Documentation updated
- [ ] No warnings/errors in build
```

### Review Process

1. **Automated checks** run (build, tests, linting)
2. **Maintainer review** within 48 hours
3. **Address feedback** (push new commits to same branch)
4. **Approval** → Squash and merge to target branch
5. **Branch cleanup** (auto-deleted after merge)

## 🧪 Testing

### Test Categories

| Category | Location | Run Command |
|----------|----------|-------------|
| Unit Tests | `Source/DA/Tests/` | `AutomationTest DA.*` |
| Integration | `Source/DACharacters/Tests/` | `AutomationTest DACharacters.*` |
| Gameplay | `Content/Tests/` | Gauntlet (future) |
| Performance | `Content/PerfTests/` | `Stat Unit`, `Stat GPU` |

### Running Tests Locally

```bash
# In Unreal Editor Automation window (Window → Automation)
# Or command line:
UnrealEditor-Cmd.exe DA.uproject -run=AutomationTest -TestNames="DA.*" -nullrhi -nosound -unattended
```

### Writing Tests

```cpp
// Source/DA/Tests/DACharacterTest.cpp
#include "Misc/AutomationTest.h"
#include "DACharacterBase.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDACharacterBaseTest, "DA.CharacterBase.CanJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FDACharacterBaseTest::RunTest(const FString& Parameters)
{
    // Test implementation
    TestTrue("Character can jump by default", Character->CanCharacterJump());
    return true;
}
```

## 📚 Documentation

### Documentation Standards

- **API Docs**: Doxygen comments on all public/protected members
- **README**: Updated for user-facing changes
- **CHANGELOG**: Maintained for each release
- **Architecture**: Update diagrams for structural changes

### Doxygen Comment Style

```cpp
/**
 * Calculates the direction of movement relative to character facing.
 * 
 * @param Velocity Current velocity vector in world space
 * @param CharacterRotation Current actor rotation
 * @return Angle in degrees (-180 to 180) where 0 = forward, 180 = backward
 * 
 * @see GetVelocity(), GetActorRotation()
 */
UFUNCTION(BlueprintPure, Category = "Movement")
float CalculateMovementDirection(const FVector& Velocity, const FRotator& CharacterRotation) const;
```

---

## 🏷 Issue Labels

| Label | Color | Description |
|-------|-------|-------------|
| `bug` | `#D73A4A` | Something isn't working |
| `enhancement` | `#A2EEEF` | New feature or request |
| `documentation` | `#0075CA` | Documentation improvements |
| `good first issue` | `#7057FF` | Good for newcomers |
| `help wanted` | `#008672` | Extra attention needed |
| `question` | `#D876E3` | Further info requested |
| `wontfix` | `#FFFFFF` | Will not be addressed |
| `blocked` | `#B60205` | Blocked by external factor |
| `performance` | `#FBCA04` | Performance related |

---

## 📞 Getting Help

- **GitHub Discussions**: For questions, ideas, design discussions
- **GitHub Issues**: For bugs, feature requests
- **Discord**: `#da-framework` on Unreal Engine Discord
- **Email**: `maintainers@da-framework.dev` (for security issues)

---

## 🙏 Recognition

Contributors are recognized in:
- `README.md` contributors section (auto-updated)
- `CHANGELOG.md` for each release
- GitHub Contributors graph
- Release notes

Thank you for contributing to DA Framework! 🎮