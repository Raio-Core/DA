// Copyright Raioix & Raiocore. All Rights Reserved 2026.

using UnrealBuildTool;
using System.Collections.Generic;

public class DAEditorTarget : TargetRules
{
	public DAEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
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

