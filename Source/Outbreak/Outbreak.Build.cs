// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Outbreak : ModuleRules
{
	public Outbreak(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"MovieScene", 
			"LevelSequence", 
			"MovieSceneTracks", 
			"UMG", 
			"EnhancedInput", 
			"NavigationSystem", 
			"AIModule",
			"Sockets",
			"Networking",
			"PacketHandler",
			"OnlineSubsystem",
			"OnlineSubsystemNull",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Paper2D" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
