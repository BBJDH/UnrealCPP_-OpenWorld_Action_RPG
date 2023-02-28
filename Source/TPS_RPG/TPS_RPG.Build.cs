// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS_RPG : ModuleRules
{
	public TPS_RPG(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayCameras" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;
	

		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.Add("Core");
		PublicDependencyModuleNames.Add("CoreUObject");
		PublicDependencyModuleNames.Add("Engine");
		PublicDependencyModuleNames.Add("InputCore");
		PublicDependencyModuleNames.Add("Niagara");
		PublicDependencyModuleNames.Add("AIModule");
		PublicDependencyModuleNames.Add("GameplayTasks");
		//포함 디렉토리 추가,  CPP_Basic 기준경로를 포함시킨다
	}
}
