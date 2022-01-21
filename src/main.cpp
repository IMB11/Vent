#include "main.hpp"

#include "UnityEngine/AssetBundle.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"

static UnityEngine::AssetBundle* assetBundle;

Configuration &getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

Logger &getLogger() {
    static auto *logger = new Logger(modInfo);
    return *logger;
}

extern "C" void setup(ModInfo &info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
    getConfig().Load();
    getLogger().info("Completed setup!");
}

UnityEngine::AssetBundle* loadAssetbundle() {
    std::string ABLocation = getDataDir(modInfo) + "Vent.ab";
    return UnityEngine::AssetBundle::LoadFromFile(il2cpp_utils::newcsstr(ABLocation));
}

void SpawnVent() {
    if(!assetBundle) return;

}

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate,
                    &GlobalNamespace::MainMenuViewController::DidActivate, void,
                    GlobalNamespace::MainMenuViewController *self,
                    bool firstActivation, bool addedToHierarchy,
                    bool screenSystemEnabling) {
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy,
                                       screenSystemEnabling);
    getLogger().info("Begin Sussyness");
    assetBundle->GetAllAssetNames();

}

extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Loading AssetBundle");
    assetBundle = loadAssetbundle();
    getLogger().info("Loaded AssetBundle");

    getLogger().info("Installing hooks...");

    getLogger().info("Installed all hooks!");
}