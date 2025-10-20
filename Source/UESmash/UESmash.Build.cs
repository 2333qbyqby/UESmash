// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UESmash : ModuleRules
{
	public UESmash(ReadOnlyTargetRules Target) : base(Target)
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
			"UESmash",
			"UESmash/Variant_Platforming",
			"UESmash/Variant_Platforming/Animation",
			"UESmash/Variant_Combat",
			"UESmash/Variant_Combat/AI",
			"UESmash/Variant_Combat/Animation",
			"UESmash/Variant_Combat/Gameplay",
			"UESmash/Variant_Combat/Interfaces",
			"UESmash/Variant_Combat/UI",
			"UESmash/Variant_SideScrolling",
			"UESmash/Variant_SideScrolling/AI",
			"UESmash/Variant_SideScrolling/Gameplay",
			"UESmash/Variant_SideScrolling/Interfaces",
			"UESmash/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
