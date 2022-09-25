#include "main.hpp"
#include "ModConfig.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "SettingsViewController.hpp"

DEFINE_TYPE(NoParticles, SettingsViewController);

using namespace GlobalNamespace;

void NoParticles::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(firstActivation)
    {
        auto SettingsContainer = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform())->get_transform();

        AddConfigValueToggle(SettingsContainer, getModConfig().DisableNotes);
        AddConfigValueToggle(SettingsContainer, getModConfig().DisableBombs);
    }
}