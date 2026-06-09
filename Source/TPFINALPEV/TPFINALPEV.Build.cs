// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPFINALPEV : ModuleRules
{
	public TPFINALPEV(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TPFINALPEV",
			"TPFINALPEV/Variant_Platforming",
			"TPFINALPEV/Variant_Platforming/Animation",
			"TPFINALPEV/Variant_Combat",
			"TPFINALPEV/Variant_Combat/AI",
			"TPFINALPEV/Variant_Combat/Animation",
			"TPFINALPEV/Variant_Combat/Gameplay",
			"TPFINALPEV/Variant_Combat/Interfaces",
			"TPFINALPEV/Variant_Combat/UI",
			"TPFINALPEV/Variant_SideScrolling",
			"TPFINALPEV/Variant_SideScrolling/AI",
			"TPFINALPEV/Variant_SideScrolling/Gameplay",
			"TPFINALPEV/Variant_SideScrolling/Interfaces",
			"TPFINALPEV/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
