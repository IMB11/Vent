#pragma once

#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "config-utils/shared/config-utils.hpp"

#include "Vent.hpp"

Configuration &getConfig();
Logger &getLogger();
static ModInfo modInfo;