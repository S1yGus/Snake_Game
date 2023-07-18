// Snake_Game, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class Snake_GameTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;

    public Snake_GameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

        ExtraModuleNames.AddRange(new string[] { "Snake_Game" });

        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
    }
}
