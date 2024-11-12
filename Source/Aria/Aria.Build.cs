// Copyright Savoie Interactive Studios

using UnrealBuildTool;

public class Aria : ModuleRules
{
	public Aria(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "BlueprintGraph", "KismetCompiler", "Kismet", "EnhancedInput", "GameplayAbilities", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[]
			{ "UnrealEd", "GraphEditor", "PropertyEditor", "GameplayTags", "GameplayTasks", "NavigationSystem", "Niagara", "AIModule" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
