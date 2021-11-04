#include "skse64/PluginAPI.h"
#include "skse64_common/skse_version.h"
#include "skse64_common/SafeWrite.h"
#include "skse64/GameAPI.h"

IDebugLog	gLog("DynamicAppearanceFramework64.log");
PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
SKSESerializationInterface* g_serialization = nullptr;
SKSEPapyrusInterface* g_papyrusInterface = nullptr;

/**** serialization ****/

const UInt32 kSerializationDataVersion = 1;

void Serialization_Revert(SKSESerializationInterface* intfc) //between save loads, revert to default value
{
}

void Serialization_Save(SKSESerializationInterface* intfc)
{
}

void Serialization_Load(SKSESerializationInterface* intfc)
{
}


extern "C"
{
	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "DynamicAppearanceFramework64";
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor)
		{
			return false;
		}
		else if (skse->runtimeVersion != RUNTIME_VERSION_1_5_97)
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

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		// register callbacks and unique ID for serialization
		// ### this must be a UNIQUE ID
		g_serialization->SetUniqueID(g_pluginHandle, 0xDAF6A2A7);

		g_serialization->SetRevertCallback(g_pluginHandle, Serialization_Revert);
		g_serialization->SetSaveCallback(g_pluginHandle, Serialization_Save);
		g_serialization->SetLoadCallback(g_pluginHandle, Serialization_Load);

		//register the papyrus functions that allow scripts to access the new event
		g_papyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);

		return true;
	}

};