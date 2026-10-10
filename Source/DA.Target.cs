// Copyright Raioix & Raiocore. All Rights Reserved 2026.

using UnrealBuildTool;
using System.Collections.Generic;

public class DATarget : TargetRules
{
	public DATarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("DA");
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[] { "DACharacters" });
	}
}

