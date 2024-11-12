// Copyright Savoie Interactive Studios

using UnrealBuildTool;
using System.Collections.Generic;

public class AriaTarget : TargetRules
{
	public AriaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Aria" } );
	}
}
