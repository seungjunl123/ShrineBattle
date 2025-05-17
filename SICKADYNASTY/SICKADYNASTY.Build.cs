// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SICKADYNASTY : ModuleRules
{
	public SICKADYNASTY(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"MotionWarping" , 
			"AnimGraphRuntime", 
			"Niagara", 
			"NavigationSystem",
			"MoviePlayer"
		});

		// GAS 모듈 사용을 위한 명시적 표기
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
