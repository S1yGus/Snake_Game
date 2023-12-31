// Snake_Game, all rights reserved.

using UnrealBuildTool;
using System.Linq;

public class Snake_Game : ModuleRules
{
    public Snake_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.ProjectDefinitions.Contains("UNOPTIMIZED_CODE"))
        {
            OptimizeCode = CodeOptimization.Never;
        }

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "SlateCore", "EnhancedInput", "Niagara" });
        PublicIncludePaths.AddRange(new string[] { "Snake_Game" });

        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PublicDependencyModuleNames.Add("FunctionalTesting");
        }
    }
}
