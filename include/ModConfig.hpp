#pragma once

#include "config-utils/shared/config-utils.hpp"


DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(DisableNotes, bool, "Disable note cut particles", true);
    CONFIG_VALUE(DisableBombs, bool, "Disable bomb cut particles", true);
    CONFIG_VALUE(DisableDust, bool, "Disables small white particles in the menu", true);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(DisableNotes);
        CONFIG_INIT_VALUE(DisableBombs);
        CONFIG_INIT_VALUE(DisableDust);

    )
)