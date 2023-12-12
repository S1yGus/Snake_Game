// Snake_Game, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class Snake_GameEditorTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;

    public Snake_GameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.AddRange(new string[] { "Snake_Game" });

        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
    }
}
