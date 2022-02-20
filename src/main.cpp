#include "main.hpp"
#include "Vent.hpp"

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

MAKE_HOOK_MATCH(SceneManager_ChangeScene,
                &SceneManagement::SceneManager::Internal_ActiveSceneChanged,
                void, SceneManagement::Scene oldScene,
                SceneManagement::Scene newScene) {
    SceneManager_ChangeScene(oldScene, newScene);
    if (!Vent::assetBundle) {
        getLogger().info("Loading AssetBundle");
        Vent::assetBundle = Vent::loadAssetbundle();
        getLogger().info("Loaded AssetBundle");
    }
    if (to_utf8(csstrtostr(newScene.get_name())) == "MainMenu") {
        getLogger().info("Begin Sussyness");
        Vent::generateVent();
    }
}

extern "C" void load() {
    il2cpp_functions::Init();
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), SceneManager_ChangeScene);
    getLogger().info("Installed all hooks!");
}