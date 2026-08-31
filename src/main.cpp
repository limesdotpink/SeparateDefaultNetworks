#include "utils/logger.h"
#include <coreinit/filesystem.h>
#include <nn/ac.h>
#include <wups.h>
#include <wups/button_combo/api.h>
#include <wups/config/WUPSConfigCategory.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemButtonCombo.h>
#include <wups/config/WUPSConfigItemIPAddress.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <wups/config/WUPSConfigItemStub.h>
#include <wups/config_api.h>

#include <malloc.h>


WUPS_PLUGIN_NAME("SeparateDefaultNetworks");
WUPS_PLUGIN_DESCRIPTION("Lets you set separate default networks for Wii U and vWii.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("limes.pink");
WUPS_PLUGIN_LICENSE("AGPLv3");

#define PLUGIN_ENABLED_CONFIG_ID "pluginEnabled"
#define WIIU_NETWORK_CONFIG_ID   "wiiUNetwork"
#define VWII_NETWORK_CONFIG_ID   "vWiiNetwork"

WUPS_USE_WUT_DEVOPTAB();
WUPS_USE_STORAGE("SeparateDefaultNetworks");


#define PLUGIN_ENABLED_DEFAULT_VALUE false
#define WIIU_NETWORK_DEFAULT_VALUE   1
#define VWII_NETWORK_DEFAULT_VALUE   2

bool sPluginEnabled = PLUGIN_ENABLED_DEFAULT_VALUE;
int sWiiUNetwork    = WIIU_NETWORK_DEFAULT_VALUE;
int sVWiiNetwork    = VWII_NETWORK_DEFAULT_VALUE;


void boolItemChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in boolItemChanged: %d", newValue);
    if (std::string_view(PLUGIN_ENABLED_CONFIG_ID) == item->identifier) {
        sPluginEnabled = newValue;
        WUPSStorageAPI::Store(item->identifier, newValue);
    }
}

void integerRangeItemChanged(ConfigItemIntegerRange *item, int newValue) {
    DEBUG_FUNCTION_LINE_INFO("New value in integerRangeItemChanged: %d", newValue);
    if (std::string_view(WIIU_NETWORK_CONFIG_ID) == item->identifier) {
        sWiiUNetwork = newValue;
        WUPSStorageAPI::Store(item->identifier, newValue);
    } else if (std::string_view(VWII_NETWORK_CONFIG_ID) == item->identifier) {
        sVWiiNetwork = newValue;
        WUPSStorageAPI::Store(item->identifier, newValue);
    }
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle) {
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    try {
        root.add(WUPSConfigItemBoolean::Create(PLUGIN_ENABLED_CONFIG_ID, "Enabled",
                                               PLUGIN_ENABLED_DEFAULT_VALUE, sPluginEnabled,
                                               boolItemChanged));

        root.add(WUPSConfigItemIntegerRange::Create(WIIU_NETWORK_CONFIG_ID, "Wii U",
                                                    WIIU_NETWORK_DEFAULT_VALUE, sWiiUNetwork,
                                                    1, 6,
                                                    &integerRangeItemChanged));

        root.add(WUPSConfigItemIntegerRange::Create(VWII_NETWORK_CONFIG_ID, "vWii",
                                                    VWII_NETWORK_DEFAULT_VALUE, sVWiiNetwork,
                                                    1, 6,
                                                    &integerRangeItemChanged));

    } catch (std::exception &e) {
        DEBUG_FUNCTION_LINE_ERR("Creating config menu failed: %s", e.what());
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void setDefaultNetworkProfiles() {
    if (sPluginEnabled) {
        nn::ac::Initialize();
        nn::ac::SetStartupId(sWiiUNetwork);
        nn::ac::SetCompatId(sVWiiNetwork);
    }
}

void ConfigMenuClosedCallback() {
    WUPSStorageAPI::SaveStorage();

    setDefaultNetworkProfiles();
}

INITIALIZE_PLUGIN() {
    initLogging();
    DEBUG_FUNCTION_LINE("INITIALIZE_PLUGIN of SeparateDefaultNetworks!");

    WUPSConfigAPIOptionsV1 configOptions = {.name = "SeparateDefaultNetworks"};
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("Failed to init config api");
    }

    WUPSStorageError storageRes;
    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault(PLUGIN_ENABLED_CONFIG_ID, sPluginEnabled, PLUGIN_ENABLED_DEFAULT_VALUE)) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("GetOrStoreDefault failed: %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }
    if ((storageRes = WUPSStorageAPI::SaveStorage()) != WUPS_STORAGE_ERROR_SUCCESS) {
        DEBUG_FUNCTION_LINE_ERR("GetOrStoreDefault failed: %s (%d)", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
    }

    setDefaultNetworkProfiles();

    deinitLogging();
}

DEINITIALIZE_PLUGIN() {
}


ON_APPLICATION_START() {
    initLogging();
}

ON_APPLICATION_ENDS() {
    deinitLogging();
}

ON_APPLICATION_REQUESTS_EXIT() {
}