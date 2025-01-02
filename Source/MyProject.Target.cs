// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProjectTarget : TargetRules
{
	public MyProjectTarget(TargetInfo target) : base(target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("MyProject");
	}
}
