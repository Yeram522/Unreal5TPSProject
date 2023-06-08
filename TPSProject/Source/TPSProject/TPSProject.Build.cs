// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPSProject : ModuleRules
{
	public TPSProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "TPSProject/Public" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","UMG", "NavigationSystem", "AIModule" });

        PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });

    }
}
