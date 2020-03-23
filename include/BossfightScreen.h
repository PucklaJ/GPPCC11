#ifndef BOSSFIGHTSCREEN_H
#define BOSSFIGHTSCREEN_H

#include "Player.h"
#include "Odin.h"
#include <vector>
#include "Defines.h"

using namespace Johnny;

class Background;

class BossfightScreen : public Actor
{
public:
    static BossfightScreen* mainBossfightScreen;
    BossfightScreen();
    ~BossfightScreen();
    
    bool init() override;
    bool update() override;
    void quit() override;
    
    void destroyGround();
    void addBoundaries();
    
    void setPlayer(Player* p) {m_player = p;}
    void setOdin(Odin* o) {m_odin = o;}
    
    Sprite2D* getGround() {return m_ground;}
    Player* getPlayer() {return m_player;}
    Odin* getOdin() {return m_odin;}
    Background* getBackground() {return m_background;}
    
private:
    void handleStates();
    void handleObstacles();
    void handleArrowPickups();
    void spawnObstacle();
    void spawnArrowPickup();

    Player* m_player = nullptr;
    Odin* m_odin = nullptr;
    Sprite2D* m_ground = nullptr;
    Background* m_background = nullptr;
    
    std::vector<Sprite2D*> m_groundDestroyParts;
    std::vector<Sprite2D*> m_obstacles;
    std::vector<Sprite2D*> m_arrowPickups;
    std::vector<float> m_arrowPickupTimes;
    
    float m_time = 0.0f;
    float m_time1 = 0.0f;
    float m_currentObstacleSpawnTime = 0.0f;
    float m_currentArrowPickupSpawnTime = 0.0f;
};  

#endif