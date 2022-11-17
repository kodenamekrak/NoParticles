#include "main.hpp"
#include "ModConfig.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "SettingsViewController.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"

#include "UnityEngine/Resources.hpp"

DEFINE_TYPE(NoParticles, SettingsViewController);

using namespace GlobalNamespace;

bool restartRequired;

void NoParticles::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if(firstActivation)
    {
        auto settingsContainer = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform())->get_transform();

        AddConfigValueToggle(settingsContainer, getModConfig().DisableNotes);
        AddConfigValueToggle(settingsContainer, getModConfig().DisableBombs);
        QuestUI::BeatSaberUI::CreateToggle(settingsContainer, "Disable floating dust particles in menu", getModConfig().DisableMenuDust.GetValue(), [&](bool value)
        {
            getModConfig().DisableMenuDust.SetValue(value, true);
            restartRequired = true;
        });
        AddConfigValueToggle(settingsContainer, getModConfig().DisableSongDust);
        AddConfigValueToggle(settingsContainer, getModConfig().DisableSaberClash);
    }
}

void NoParticles::SettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
{
    if(restartRequired)
        UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MenuTransitionsHelper *>().First()->RestartGame(nullptr);
}