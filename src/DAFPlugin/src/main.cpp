#include "PluginAPI.h"
#include "skse_version.h"
#include "SafeWrite.h"
#include "GameAPI.h"

#include "parse/MockConfigurationParser.h"
#include "PapyrusSandbox.h"
#include "chargen/Chargen.h"
#include "chargen/MorphHandler.h"
extern MorphHandler g_morphHandler;

IDebugLog	gLog("DynamicAppearanceFramework.log");
PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
SKSESerializationInterface* g_serialization = NULL;
SKSEPapyrusInterface* g_papyrusInterface = NULL;

// TODO: move serialization into its own file
#pragma region Serialization
const UInt32 kSerializationDataVersion = 1;

void Serialization_Revert(SKSESerializationInterface* intfc)
{
	g_morphHandler.Revert();
}

void Serialization_Save(SKSESerializationInterface* intfc)
{
	_DMESSAGE("Saving...");

	/*PlayerCharacter* player = (*g_thePlayer);
	TESNPC* playerBase = DYNAMIC_CAST(player->baseForm, TESForm, TESNPC);

	g_morphHandler.Save(playerBase, intfc, kSerializationDataVersion);*/

	_DMESSAGE("Save Complete.");
}

void Serialization_Load(SKSESerializationInterface* intfc)
{
	_DMESSAGE("Loading...");

	/*PlayerCharacter* player = (*g_thePlayer);
	TESNPC* playerBase = DYNAMIC_CAST(player->baseForm, TESForm, TESNPC);

	g_morphHandler.Load(playerBase, intfc, kSerializationDataVersion);

	g_task->AddTask(new SKSETaskApplyMorphs(player));*/

	_DMESSAGE("Load Complete.");
}
}
#pragma endregion

#pragma region Plugin Interface

extern "C"
{
#if defined AE_BUILD
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,
		1,
		"DynamicAppearanceFrameworkAE",
		"Jake Alaimo",
		"",
		0,	// not version independent - perhaps might want to do something about that
		{ RUNTIME_VERSION_1_6_353, 0 },
		0,
	};

#else
	__declspec(dllexport) bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "DynamicAppearanceFramework";
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		UINT32 targetRuntime = 0;
#ifdef LE_BUILD
		targetRuntime = RUNTIME_VERSION_1_9_32_0;
#else
		targetRuntime = RUNTIME_VERSION_1_5_97;
		info->name = "DynamicAppearanceFrameworkSE";
#endif

		if (skse->isEditor)
		{
			return false;
		}
		else if (skse->runtimeVersion != targetRuntime)
		{
			return false;
		}

		// get the serialization interface and query its version
		g_serialization = (SKSESerializationInterface*)skse->QueryInterface(kInterface_Serialization);
		if (!g_serialization)
		{
			return false;
		}
		if (g_serialization->version < SKSESerializationInterface::kVersion)
		{
			return false;
		}

		// call chargen query callback
		Chargen_Query(skse, info);

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}
#endif

	__declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
#ifdef AE_BUILD
		// in the case of AE, plugin handle and interfaces are not grabbed yet as SKSEPlugin_Query never fires
		g_pluginHandle = skse->GetPluginHandle();
		g_serialization = (SKSESerializationInterface*)skse->QueryInterface(kInterface_Serialization);
#endif // AE_BUILD

		// register callbacks and unique ID for serialization
		// ### this must be a UNIQUE ID
		g_serialization->SetUniqueID(g_pluginHandle, 0xDAF6A2A7);

		g_serialization->SetRevertCallback(g_pluginHandle, Serialization_Revert);
		g_serialization->SetSaveCallback(g_pluginHandle, Serialization_Save);
		g_serialization->SetLoadCallback(g_pluginHandle, Serialization_Load);

		// prepare the papyrus function environment - doesn't need to happen here
		std::shared_ptr<DAF::MockConfigurationParser> mockParser = std::make_shared<DAF::MockConfigurationParser>();

		PapyrusSandbox::SandboxDependencies dependencies;
		dependencies.stateRegistry = std::make_shared<ActorStateRegistry>(mockParser);
		dependencies.transformationManager = std::make_shared<DAF::TraitTransformationManager>(*mockParser);
		PapyrusSandbox::ConfigureSandbox(dependencies);

		// register the papyrus functions that allow scripts to access the framework
		g_papyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		g_papyrusInterface->Register(PapyrusSandbox::RegisterFunctions);

		// call chargen load callback
		Chargen_Load(skse);

		return true;
	}
};

#pragma endregion