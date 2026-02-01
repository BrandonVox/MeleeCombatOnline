// Copyright Brandon Vox.

using UnrealBuildTool;
using System.Collections.Generic;

public class MeleeCombatOnlineEditorTarget : TargetRules
{
	public MeleeCombatOnlineEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MeleeCombatOnline" } );
	}
}
