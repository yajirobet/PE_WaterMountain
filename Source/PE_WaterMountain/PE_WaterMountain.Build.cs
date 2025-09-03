// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PE_WaterMountain : ModuleRules
{
	public PE_WaterMountain(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"ChaosVehicles",
			"PhysicsCore",
			"UMG",
			"Slate"
		});

		PublicIncludePaths.AddRange(new string[] {
			"PE_WaterMountain",
			"PE_WaterMountain/SportsCar",
			"PE_WaterMountain/OffroadCar",
			"PE_WaterMountain/Variant_Offroad",
			"PE_WaterMountain/Variant_TimeTrial",
			"PE_WaterMountain/Variant_TimeTrial/UI"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
