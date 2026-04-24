// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ecos_del_Abismo : ModuleRules
{
	public Ecos_del_Abismo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
