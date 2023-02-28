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
		//���� ���丮 �߰�,  CPP_Basic ���ذ�θ� ���Խ�Ų��
	}
}
