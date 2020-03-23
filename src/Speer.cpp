#include "../include/Speer.h"
#include "../include/Defines.h"
#include "../include/Player.h"
#include <Johnny.h>

Speer::Speer(const Vector2f& position,const Vector2f& velocity) : Sprite2D("speer.png"),
    m_velocity(velocity)
{
    setPosition(position);
    setName("Speer");
}

Speer::~Speer()
{
    
}

bool Speer::init()
{
    Sprite2D::init();
    setRotation(m_velocity.angle());
    setScale(GAME_SCALE,GAME_SCALE);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    setDepth(SPEER_DEPTH);
    
    m_texture->setFlip(Flip::HORIZONTALLY);
    
    return true;
}

bool Speer::update()
{
    Sprite2D::update();
    
    controlHitPlayer();
    controlPhysics();
    
    return true;
}

void Speer::quit()
{
    Sprite2D::quit();
}

void Speer::controlPhysics()
{
    addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
    
    if(getPosition().x > m_mainClass->getNativeRes().x + SPEER_X_POS_DISAPPEAR)
        getParent()->removeChild(this);
    else if(getPosition().x < -SPEER_X_POS_DISAPPEAR)
        getParent()->removeChild(this);
    else if(getPosition().y > m_mainClass->getNativeRes().y + SPEER_Y_POS_DISAPPEAR)
        getParent()->removeChild(this);
    else if(getPosition().y < -SPEER_Y_POS_DISAPPEAR)
        getParent()->removeChild(this);
}

void Speer::controlHitPlayer()
{    
    if(Player::mainPlayer && Player::mainPlayer->m_state != PLAYER_KNOCKBACK_STATE && Player::mainPlayer->intersects(getPosition()))
    {
        if(m_velocity.angle() < 180.0f)
            Player::mainPlayer->hitBySpeer(getPosition());
        else
            Player::mainPlayer->hitBySpeer(getPosition()+getActualSize()/2.0f);
        getParent()->removeChild(this);
    }
}