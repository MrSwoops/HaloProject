using UnrealBuildTool;

public class MyProjectEditor : ModuleRules
{
	public MyProjectEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core", "CoreUObject", "Engine", "UnrealEd", "AssetTools", "MyProject"
			}
		);
	}
}

//Add to uproject to readd
// ,
// {
// 	"Name": "MyProjectEditor",
// 	"Type": "Editor",
// 	"LoadingPhase": "Default"
// }