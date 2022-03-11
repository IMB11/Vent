#include "main.hpp"

#include "UnityEngine/AssetBundle.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "System/Action_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Camera.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "UnityEngine/BoxCollider.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/Sprite.hpp"
#include "GlobalNamespace/PersistentSingleton_1.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/NoTransitionsButton.hpp"
#include "HMUI/Touchable.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;

#define MakeDelegate(DelegateType, varName) (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

static AssetBundle* assetBundle;

static GameObject* vent;
static const float ventAnimTime = 0.4f;

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

AssetBundle* loadAssetbundle() {
    return AssetBundle::LoadFromFile("/sdcard/ModData/com.beatgames.beatsaber/Mods/Vent/Vent.ab");
}

float easeInOutExpo(float start, float end, float value) {
    value /= .5f;
    end -= start;
    if (value < 1) return end * 0.5f * value * value * value * value * value + start;
    value -= 2;
    return end * 0.5f * (value * value * value * value * value + 2) + start;
}

custom_types::Helpers::Coroutine ventStuff(RectTransform* ventTop)
{
    GameObject* o = GameObject::Find("Origin");
    Vector3 sp = o->get_transform()->get_position();
    Transform* transform = o->GetComponentInChildren<Camera *>()->get_transform();
    Vector3 offs = ventTop->get_position() - transform->get_position();
    Vector3 offsVented = ventTop->get_position() - transform->get_position();
    offs.z -= 0.4f;
    offs.x += 0.1f;
    offs.y = 0.3f;
    offsVented.y = -3.0f;
    for (float t = 0.0f; t < 0.4f; t += Time::get_deltaTime()) {
        float num = easeInOutExpo(0.0f, 1.0f, t / 0.4f);
        if (o->get_activeInHierarchy()) {
            o->get_transform()->set_position(Vector3::Lerp(sp, sp + offs, num));
        }
        ventTop->set_localEulerAngles(Vector3(90.0f + num * 100.0f, 0.0f, 0.0f));
        co_yield nullptr;
    }
    ventTop->set_localEulerAngles(Vector3(190.0f, 0.0f, 0.0f));
    for (float t = 0.0f; t < 0.4f; t += Time::get_deltaTime())
    {
        float num2 = easeInOutExpo(0.0f, 1.0f, t / 0.4f);
        if (o->get_activeInHierarchy())
        {
            o->get_transform()->set_position(Vector3::Lerp(sp + offs, sp + offsVented, num2 * 1.5f));
            o->get_transform()->set_localScale(Vector3::Lerp(Vector3::get_one(), Vector3::get_zero(), num2 * 0.3f));
        }
        ventTop->set_localEulerAngles(Vector3(190.0f - num2 * 100.0f, 0.0f, 0.0f));
        co_yield nullptr;
    }
    Application::Quit();
    co_return;
}

void SpawnVent() {
    if(!assetBundle) return;
    if(vent) return;

    Material* material = QuestUI::ArrayUtil::First(Resources::FindObjectsOfTypeAll<Material*>(), [](Material* x) { return x->get_name() == "UINoGlow"; });
    GameObject* gameObject = GameObject::New_ctor("VentCanvas");
    gameObject->AddComponent<RectTransform *>();
    gameObject->AddComponent<HMUI::Touchable *>();
    gameObject->AddComponent<Canvas *>();

    RectTransform* parent = reinterpret_cast<RectTransform *>(gameObject->get_transform());
    HMUI::ImageView* imageView = GameObject::New_ctor("Inner")->AddComponent<HMUI::ImageView *>();
    RectTransform*l = reinterpret_cast<RectTransform *>(imageView->get_transform());

    l->set_localEulerAngles(Vector3(90.0f, 0.0f, 0.0f));
    l->set_position(Vector3(0.0f, 0.004f, 0.0f));
    l->set_sizeDelta(Vector2(0.512f, 0.373f));
    l->get_transform()->set_parent(parent);
    l->set_pivot(Vector2(0.5f, 1.0f));

    imageView->set_material(material);
    imageView->set_sprite(assetBundle->LoadAsset<Sprite *>("vent_inner"));

    HMUI::ImageView* ventTop = GameObject::New_ctor("Top")->AddComponent<HMUI::ImageView *>();
    ventTop->get_gameObject()->AddComponent<BoxCollider *>();
    ventTop->get_gameObject()->AddComponent<HMUI::NoTransitionsButton *>();
    l = reinterpret_cast<RectTransform *>(ventTop->get_transform());
    l->set_localEulerAngles(Vector3(90.0f, 0.0f, 0.0f));
    l->set_position(Vector3(0.0f, 0.005f, 0.0f));
    l->set_sizeDelta(Vector2(0.512f, 0.373f));
    l->get_transform()->set_parent(parent);
    l->set_pivot(Vector2(0.5f, 1.0f));
    ventTop->set_material(material);
    ventTop->set_sprite(assetBundle->LoadAsset<Sprite *>("vent_top"));
    vent = GameObject::New_ctor("SusVent");
    Transform* transform = vent->get_transform();

    GameObject* menuCore = GameObject::Find("MenuCore");
    transform->set_parent(menuCore->get_transform());
    Object::DontDestroyOnLoad(vent);
    gameObject->get_transform()->set_parent(vent->get_transform());

    transform->set_position(Vector3(0.4f, 0.1f, -0.9f));
    transform->set_eulerAngles(Vector3(0.0f, -3.0f, 0.0f));

    HMUI::NoTransitionsButton* lmao = ventTop->get_gameObject()->GetComponent<HMUI::NoTransitionsButton *>();
    AudioSource* h = vent->AddComponent<AudioSource *>();
    h->set_clip(assetBundle->LoadAsset<AudioClip *>("vent_open"));
    h->set_playOnAwake(false);
    lmao->set_onClick(UI::Button::ButtonClickedEvent::New_ctor());
    std::function<void()> lmao_onClick = [h, l] () {
        h->Play();
        getLogger().info("Vent Clicked!");
        SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(ventStuff(l)));
    };

    ventTop->set_color({0.7f, 0.7f, 0.7f, 1.0f});

    std::function<void(HMUI::NoTransitionsButton::SelectionState)> lmao_sschg = [ventTop] (HMUI::NoTransitionsButton::SelectionState ss) {
        if(ss != 1) {
            ventTop->set_color({0.7f, 0.7f, 0.7f, 1.0f});
        } else {
            ventTop->set_color(Color::get_white());
        }
    };

    lmao->get_onClick()->AddListener(MakeDelegate(Events::UnityAction *, lmao_onClick));

    using delType = System::Action_1<HMUI::NoTransitionsButton::SelectionState> *;
    lmao->add_selectionStateDidChangeEvent(MakeDelegate(delType, lmao_sschg));

    BoxCollider* component = ventTop->GetComponent<BoxCollider*>();
    component->set_center(Vector3(0.0f, -0.21f, 0.0f));
    component->set_size(Vector3(0.48f, 0.33f, 0.1f));
    assetBundle->Unload(false);
}

MAKE_HOOK_MATCH(SceneManager_ChangeScene,
                    &SceneManagement::SceneManager::Internal_ActiveSceneChanged, void,
                    SceneManagement::Scene oldScene, SceneManagement::Scene newScene) {
    SceneManager_ChangeScene(oldScene, newScene);
    if(!assetBundle) {
        getLogger().info("Loading AssetBundle");
        assetBundle = loadAssetbundle();
        getLogger().info("Loaded AssetBundle");
    }
    if(newScene.get_name() == "MainMenu") {
        getLogger().info("Begin Sussyness");
        SpawnVent();
    }

}

extern "C" void load() {
    il2cpp_functions::Init();
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), SceneManager_ChangeScene);
    getLogger().info("Installed all hooks!");
}