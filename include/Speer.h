#ifndef SPEER_H
#define SPEER_H

#include <Sprite2D.h>

using namespace Johnny;

class Player;

class Speer : public Sprite2D
{
public:
    friend class Player;

    Speer(const Vector2f& position,const Vector2f& velocity);
    ~Speer();
    
    bool init() override;
    bool update() override;
    void quit() override;
private:
    void controlPhysics();
    void controlHitPlayer();

    Vector2f m_velocity;
};

#endif