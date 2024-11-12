// Copyright Savoie Interactive Studios

using UnrealBuildTool;
using System.Collections.Generic;

public class AriaEditorTarget : TargetRules
{
	public AriaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Aria" } );
	}
}
