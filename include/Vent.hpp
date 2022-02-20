#pragma once

#include "main.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/NoTransitionsButton.hpp"
#include "HMUI/Touchable.hpp"
#include "System/Action_1.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/AssetBundle.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/BoxCollider.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "questui/shared/ArrayUtil.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;

#define MakeDelegate(DelegateType, varName)                                    \
    (il2cpp_utils::MakeDelegate<DelegateType>(classof(DelegateType), varName))

namespace Vent {
    static AssetBundle *assetBundle;
    static GameObject *vent;

    custom_types::Helpers::Coroutine activateVent(RectTransform *ventTop);
    void generateVent();
    AssetBundle *loadAssetbundle();

    class Utils {
        public:
          static float easeInOutExpo(float start, float end, float value) {
            value /= 5;
            end -= start;
            if (value < 1.0f) {
                return end * 0.5f * Mathf::Pow(2.0f, 10.0f * (value - 1.0f)) + start;
            }
            value -= 1.0f;
            return end * 0.5f * (-Mathf::Pow(2.0f, 10.0f * (value - 1.0f))) + start;
        }
    };
}