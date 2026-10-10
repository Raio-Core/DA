# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Comprehensive README.md with architecture diagrams, setup guides, and contributing guidelines
- CONTRIBUTING.md with detailed development workflow and coding standards
- GitHub Actions CI/CD pipeline (`.github/workflows/ci.yml`)
- MIT License
- Code quality checks (clang-format, include-what-you-use)
- Automated documentation generation with Doxygen
- README auto-update workflow for contributor stats

### Changed
- Fixed Enhanced Input subsystem usage in `DAPlayerCharacter.cpp`
- Corrected movement axis mapping (X=strafe, Y=forward/back)
- Added `Super::SetupPlayerInputComponent()` call
- Added `EnhancedInputSubsystems.h` include

### Fixed
- Compilation error: `AddMappingContext` not a member of `UEnhancedInputComponent`
- Compilation error: `DefaultMappingContexts` undeclared identifier
- Movement direction inversion (D key moved forward, A key moved backward)

## [0.1.0] - 2026-10-10

### Added
- Initial project structure with DA and DACharacters modules
- DACharacterBase base class with jump functionality
- DAPlayerCharacter with dual camera system (First/Third person)
- Enhanced Input integration with mapping contexts
- Four gameplay variants scaffolded:
  - Third Person
  - Combat
  - Platforming
  - Side Scrolling
- Input actions: Move, Look, Jump, ToggleCameraPerspective
- Default Input Mapping Context (IMC_Default)
- Project configuration files (DefaultEngine.ini, DefaultGame.ini, etc.)
- Basic level prototyping assets
- Character mannequin assets (Manny/Quinn)
- .gitignore for Unreal Engine projects

### Technical Details
- Unreal Engine 5.8
- C++20 standard
- Visual Studio 2022 17.8+ toolchain
- Windows 10 SDK 10.0.22621
- Enhanced Input plugin
- StateTree & GameplayStateTree plugins
- Modular plugin architecture

---

## Release Template

### [X.Y.Z] - YYYY-MM-DD

#### Added
- Feature descriptions

#### Changed
- Changes to existing functionality

#### Deprecated
- Soon-to-be removed features

#### Removed
- Removed features

#### Fixed
- Bug fixes

#### Security
- Vulnerability fixes