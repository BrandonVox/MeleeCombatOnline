// Copyright Brandon Vox.

using UnrealBuildTool;
using System.Collections.Generic;

public class MeleeCombatOnlineTarget : TargetRules
{
	public MeleeCombatOnlineTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MeleeCombatOnline" } );
	}
}
