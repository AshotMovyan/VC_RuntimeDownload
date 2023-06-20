// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class HTTP_request : ModuleRules
{
	public HTTP_request(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP" , "RuntimeArchiver" });

       // PrivateDependencyModuleNames.Add("zlib");

        //PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "miniz"));
        //PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty", "miniz", "miniz.lib"));

        PrivateDependencyModuleNames.AddRange(new string[] {  });
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
