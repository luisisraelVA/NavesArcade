// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NavesArcade : ModuleRules
{
    public NavesArcade(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Combinamos UMG (para la interfaz) con AIModule y GameplayTasks (para los enemigos)
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore",
            "AIModule",
            "GameplayTasks"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
