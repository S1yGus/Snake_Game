// Snake_Game, all rights reserved.

using UnrealBuildTool;
using System.Linq;

public class Snake_Game_Tests : ModuleRules
{
    public Snake_Game_Tests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "FunctionalTesting", "Snake_Game" });
        PublicIncludePaths.AddRange(new string[] { "Snake_Game_Tests", "Snake_Game" });
    }
}
