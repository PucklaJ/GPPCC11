#ifndef SOUNDS_H
#define SOUNDS_H

#include <cAudio/cAudio.h>

namespace Sounds
{
    extern void load(cAudio::IAudioManager* mgr);
    
    extern cAudio::IAudioSource* mainTheme;
    extern cAudio::IAudioSource* introTheme;
    extern cAudio::IAudioSource* pain[6];
    extern cAudio::IAudioSource* sword[2];
    extern cAudio::IAudioSource* bow;
    extern cAudio::IAudioSource* arrowHit[8];
    extern cAudio::IAudioSource* walkWood[2];
    extern cAudio::IAudioSource* jump[4];
    extern cAudio::IAudioSource* arrowPickUp;
    extern cAudio::IAudioSource* odinPain;
    extern cAudio::IAudioSource* knockbackExplosion;
    extern cAudio::IAudioSource* speer;
    extern cAudio::IAudioSource* axe[2];
    extern cAudio::IAudioSource* lightning_throw[2];
    extern cAudio::IAudioSource* lightning;
    extern cAudio::IAudioSource* groundDestruction[4];
}

#endif