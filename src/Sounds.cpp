#include "../include/Sounds.h"
#include "../include/Defines.h"
#include <string>


namespace Sounds
{
    void load(cAudio::IAudioManager* mgr)
    {
        Sounds::mainTheme = mgr->create("mainTheme","res/sounds/mainTheme.wav");
        Sounds::introTheme = mgr->create("introTheme","res/sounds/introTheme.ogg");
        for(int i = 0;i<6;i++)
            Sounds::pain[i] = mgr->create((std::string("pain") + std::to_string(i+1)).c_str(),(std::string("res/sounds/pain") + std::to_string(i+1) + ".wav").c_str(),true);
        Sounds::sword[0] = mgr->create("sword1","res/sounds/sword1.wav",true);
        Sounds::sword[1] = mgr->create("sword2","res/sounds/sword2.wav",true);
        Sounds::bow = mgr->create("bow","res/sounds/bow.ogg",true);
        for(int i = 0;i<8;i++)
            Sounds::arrowHit[i] = mgr->create((std::string("arrowHit") + std::to_string(i+1)).c_str(),(std::string("res/sounds/arrowHit0") + std::to_string(i+1) + ".wav").c_str(),true);
        for(int i = 0;i<2;i++)
            Sounds::walkWood[i] = mgr->create((std::string("stepwood_") + std::to_string(i+1)).c_str(),(std::string("res/sounds/stepwood_") + std::to_string(i+1) + ".wav").c_str(),true);
        for(int i = 0;i<4;i++)
            Sounds::jump[i] = mgr->create((std::string("jump") + std::to_string(i+1)).c_str(),(std::string("res/sounds/jump") + std::to_string(i+1) + ".ogg").c_str(),true);
        Sounds::arrowPickUp = mgr->create("arrowPickUp","res/sounds/arrowPickUp.wav",true);
        Sounds::odinPain = mgr->create("odinPain","res/sounds/odinPain.wav",true);
        Sounds::knockbackExplosion = mgr->create("knockbackExplosion","res/sounds/knockbackExplosion.wav",true);
        Sounds::speer = mgr->create("speer","res/sounds/speer.wav",true);
        for(int i = 0;i<2;i++)
            Sounds::axe[i] = mgr->create((std::string("axe") + std::to_string(i+1)).c_str(),(std::string("res/sounds/axe") + std::to_string(i+1) + ".ogg").c_str(),true);
        for(int i = 0;i<2;i++)
            Sounds::lightning_throw[i] = mgr->create((std::string("lightning_throw") + std::to_string(i+1)).c_str(),(std::string("res/sounds/lightning_throw") + std::to_string(i+1) + ".ogg").c_str(),true);
        Sounds::lightning = mgr->create("lightning","res/sounds/lightning.ogg",true);
        for(int i = 0;i<4;i++)
            Sounds::groundDestruction[i] = mgr->create((std::string("groundDestruction") + std::to_string(i+1)).c_str(),(std::string("res/sounds/groundDestruction") + std::to_string(i+1) + ".ogg").c_str(),true);
        
        Sounds::mainTheme->setVolume(DEFAULT_VOLUME);
        Sounds::introTheme->setVolume(DEFAULT_VOLUME);
        for(int i = 0;i<6;i++)
            Sounds::pain[i]->setVolume(DEFAULT_VOLUME/2.0f);
        for(int i = 0;i<2;i++)
            Sounds::sword[i]->setVolume(DEFAULT_VOLUME/2.0f);
        Sounds::bow->setVelocity(DEFAULT_VOLUME/5.0f);
        for(int i = 0;i<8;i++)
            Sounds::arrowHit[i]->setVolume(DEFAULT_VOLUME/2.0f);
        for(int i = 0;i<2;i++)
            Sounds::walkWood[i]->setVolume(DEFAULT_VOLUME/2.0f);
        for(int i = 0;i<4;i++)
            Sounds::jump[i]->setVolume(DEFAULT_VOLUME/2.0f);
        Sounds::arrowPickUp->setVolume(DEFAULT_VOLUME/2.0f);
        Sounds::odinPain->setVolume(DEFAULT_VOLUME/2.0f);
        Sounds::knockbackExplosion->setVolume(DEFAULT_VOLUME/2.0f);
        Sounds::speer->setVolume(DEFAULT_VOLUME/1.5f);
        for(int i = 0;i<2;i++)
            Sounds::axe[i]->setVolume(DEFAULT_VOLUME/1.5f);
        for(int i = 0;i<2;i++)
            Sounds::lightning_throw[i]->setVolume(DEFAULT_VOLUME/2.0f);
        Sounds::lightning->setVolume(DEFAULT_VOLUME/2.0f);
        for(int i = 0;i<4;i++)
            Sounds::groundDestruction[i]->setVolume(DEFAULT_VOLUME/2.0f);
        
        Sounds::bow->setPitch(1.3f);
        Sounds::knockbackExplosion->setPitch(2.0f);
    }
    
    cAudio::IAudioSource* mainTheme = nullptr;
    cAudio::IAudioSource* introTheme = nullptr;
    cAudio::IAudioSource* pain[6];
    cAudio::IAudioSource* sword[2];
    cAudio::IAudioSource* bow = nullptr;
    cAudio::IAudioSource* arrowHit[8];
    cAudio::IAudioSource* walkWood[2];
    cAudio::IAudioSource* jump[4];
    cAudio::IAudioSource* arrowPickUp = nullptr;
    cAudio::IAudioSource* odinPain = nullptr;
    cAudio::IAudioSource* knockbackExplosion = nullptr;
    cAudio::IAudioSource* speer = nullptr;
    cAudio::IAudioSource* axe[2];
    cAudio::IAudioSource* lightning_throw[2];
    cAudio::IAudioSource* lightning = nullptr;
    cAudio::IAudioSource* groundDestruction[4];
}