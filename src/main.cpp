#include "main.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "questui/shared/QuestUI.hpp"
#include "SettingsViewController.hpp"
#include "ModConfig.hpp"

#include "GlobalNamespace/NoteCutParticlesEffect.hpp"
#include "GlobalNamespace/BombExplosionEffect.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Shader.hpp"

DEFINE_CONFIG(ModConfig);

using namespace GlobalNamespace;


static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
// other config tools such as config-utils don't use this config, so it can be removed if those are in use
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

MAKE_HOOK_MATCH(SceneManager_Internal_ActiveSceneChanged, &UnityEngine::SceneManagement::SceneManager::Internal_ActiveSceneChanged, void, UnityEngine::SceneManagement::Scene previousActiveScene, UnityEngine::SceneManagement::Scene newActiveScene)
{
    SceneManager_Internal_ActiveSceneChanged(previousActiveScene, newActiveScene);

    if(getModConfig().DisableDust.GetValue())
        for (auto Particle : UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::ParticleSystem *>())
        {
            if(Particle->get_name() == "DustPS")
            {
                Particle->get_gameObject()->SetActive(false);
                break;
            }
        }
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
    INSTALL_HOOK(getLogger(), SceneManager_Internal_ActiveSceneChanged);
    getLogger().info("Installed all hooks!");
}