#include "main.hpp"
#include "questui/shared/QuestUI.hpp"
#include "SettingsViewController.hpp"
#include "ModConfig.hpp"

#include "GlobalNamespace/NoteCutParticlesEffect.hpp"
#include "GlobalNamespace/BombExplosionEffect.hpp"
#include "GlobalNamespace/SaberClashEffect.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "UnityEngine/Resources.hpp"

using namespace GlobalNamespace;


static ModInfo modInfo;

Configuration &getConfig()
{
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}

MAKE_HOOK_MATCH(SceneManager_SetActiveScene, &UnityEngine::SceneManagement::SceneManager::SetActiveScene, bool, UnityEngine::SceneManagement::Scene newActiveScene)
{
    bool result = SceneManager_SetActiveScene(newActiveScene);

    std::string sceneName = newActiveScene.get_name();

    if((sceneName == "MainMenu" && getModConfig().DisableMenuDust.GetValue()) || (sceneName == "GameCore" && getModConfig().DisableSongDust.GetValue()))
    {
        for (auto particle : UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::ParticleSystem *>())
        {
            if(particle->get_name() == "DustPS")
            {
                particle->get_gameObject()->SetActive(false);
            }
        }
    }
    
    return result;
}

MAKE_HOOK_MATCH(SaberClashEffect_LateUpdate, &SaberClashEffect::Start, void, SaberClashEffect *self)
{
    if(getModConfig().DisableSaberClash.GetValue())
    {
        self->sparkleParticleSystem->get_gameObject()->SetActive(false);
        self->glowParticleSystem->get_gameObject()->SetActive(false);
    }
    SaberClashEffect_LateUpdate(self); 
}

MAKE_HOOK_MATCH(NoteCutParticlesEffect_SpawnParticles, &NoteCutParticlesEffect::SpawnParticles, void, NoteCutParticlesEffect *self, UnityEngine::Vector3 cutPoint, UnityEngine::Vector3 cutNormal, UnityEngine::Vector3 saberDir, float saberSpeed, UnityEngine::Vector3 noteMovementVec, UnityEngine::Color32 color, int sparkleParticlesCount, int explosionParticlesCount, float lifetimeMultiplier)
{
    if (getModConfig().DisableNotes.GetValue())
    {
        sparkleParticlesCount = 0;
        explosionParticlesCount = 0;
    }
    NoteCutParticlesEffect_SpawnParticles(self, cutPoint, cutNormal, saberDir, saberSpeed, noteMovementVec, color, sparkleParticlesCount, explosionParticlesCount, lifetimeMultiplier);
}

MAKE_HOOK_MATCH(BombExplosionEffect_SpawnExplosion, &BombExplosionEffect::SpawnExplosion, void, BombExplosionEffect *self, UnityEngine::Vector3 pos)
{
    if (getModConfig().DisableBombs.GetValue())
    {
        self->debrisCount = 0;
        self->explosionParticlesCount = 0;
    }
    BombExplosionEffect_SpawnExplosion(self, pos);
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo &info)
{
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load()
{
    il2cpp_functions::Init();

    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController<NoParticles::SettingsViewController*>(modInfo, "NoParticles");

    getModConfig().Init(modInfo);

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), NoteCutParticlesEffect_SpawnParticles);
    INSTALL_HOOK(getLogger(), BombExplosionEffect_SpawnExplosion);
    INSTALL_HOOK(getLogger(), SceneManager_SetActiveScene);
    INSTALL_HOOK(getLogger(), SaberClashEffect_LateUpdate);
    getLogger().info("Installed all hooks!");
}