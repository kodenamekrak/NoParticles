#include "main.hpp"
#include "ModConfig.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"
#include "config-utils/shared/config-utils.hpp"
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
        auto SettingsContainer = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform())->get_transform();

        AddConfigValueToggle(SettingsContainer, getModConfig().DisableNotes);
        AddConfigValueToggle(SettingsContainer, getModConfig().DisableBombs);
        QuestUI::BeatSaberUI::CreateToggle(SettingsContainer, "Disable Floating dust particles", getModConfig().DisableDust.GetValue(), [&](bool value)
        {
            getModConfig().DisableDust.SetValue(value, true);
            restartRequired = true;
        });
    }
}

void NoParticles::SettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
{
    if(restartRequired)
        UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::MenuTransitionsHelper *>()[0]->RestartGame(nullptr);
}