# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 0.1.x   | ✅ Yes             |
| < 0.1   | ❌ No              |

## Reporting a Vulnerability

We take security seriously. If you discover a security vulnerability, please report it responsibly.

### How to Report

**Please do NOT open a public GitHub issue for security vulnerabilities.**

Instead, please email us at: **security@da-framework.dev**

Include the following information:
- Description of the vulnerability
- Steps to reproduce
- Potential impact
- Suggested fix (if any)
- Your contact information

### What to Expect

1. **Acknowledgment**: Within 48 hours
2. **Assessment**: Within 5 business days
3. **Fix Timeline**: Communicated after assessment
4. **Disclosure**: Coordinated with reporter

## Security Best Practices

### For Contributors

- Never commit secrets, API keys, or credentials
- Use GitHub Secrets for CI/CD credentials
- Enable 2FA on your GitHub account
- Keep dependencies updated (Dependabot handles this)

### For Users

- Always build from source or verified releases
- Verify checksums of downloaded packages
- Keep Unreal Engine updated to latest security patches
- Review plugin permissions before installing

## Scope

This security policy covers:
- Core DA Framework code (`Source/DA/`, `Source/DACharacters/`)
- Build and packaging scripts
- GitHub Actions workflows
- Documentation

Out of scope:
- Unreal Engine itself (report to Epic Games)
- Third-party plugins (report to plugin authors)
- User-generated content

## Recognition

Security researchers who report valid vulnerabilities will be:
- Credited in the release notes (with permission)
- Added to our Hall of Fame (coming soon)
- Eligible for swag (stickers, etc.)

---

**PGP Key** (for encrypted reports):
```
-----BEGIN PGP PUBLIC KEY BLOCK-----
[Key will be added when available]
-----END PGP PUBLIC KEY BLOCK-----
```