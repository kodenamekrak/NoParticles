#pragma once

#include "config-utils/shared/config-utils.hpp"


DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(DisableNotes, bool, "Disable note cut particles", true);
    CONFIG_VALUE(DisableBombs, bool, "Disable bomb cut particles", true);
    CONFIG_VALUE(DisableMenuDust, bool, "Disable floating dust particles in menu", true);
    CONFIG_VALUE(DisableSongDust, bool, "Disable floating dust particles in song", true);
    CONFIG_VALUE(DisableSaberClash, bool, "Disable saber clash effect", true);
)