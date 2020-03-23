#include "../include/Arrow.h"
#include "../include/Defines.h"
#include "../include/Odin.h"
#include "../include/Game.h"
#include "../include/Sounds.h"
#include <Johnny.h>

Arrow::Arrow(const Vector2f& position,const Vector2f& direction) : Sprite2D("arrow.png")
{
    setPosition(position);
    m_velocity = direction * ARROW_VELOCITY;
}

Arrow::~Arrow()
{
    
}

bool Arrow::init()
{
    Sprite2D::init();
    
    setScale(GAME_SCALE,GAME_SCALE);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    setDepth(ARROW_DEPTH);
    
    if(m_velocity.x > 0)
        m_texture->setFlip(Flip::HORIZONTALLY);
    else
        m_texture->setFlip(Flip::NONE);
    
    return true;
}

bool Arrow::update()
{
    Sprite2D::update();
    
    checkOdin();
    controlPhysics();
    
    return true;
}

void Arrow::quit()
{
    Sprite2D::quit();
}

void Arrow::controlPhysics()
{
    addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
    
    if(getPosition().y < -SPEER_Y_POS_DISAPPEAR ||
       getPosition().y > m_mainClass->getNativeRes().height + SPEER_Y_POS_DISAPPEAR ||
       getPosition().x < -SPEER_X_POS_DISAPPEAR ||
       getPosition().x > m_mainClass->getNativeRes().width + SPEER_X_POS_DISAPPEAR)
    {
        getParent()->removeChild(this);
    }
}

void Arrow::checkOdin()
{
    if((Game::state == GAME_BOSS_GROUND_STATE ||
        Game::state == GAME_BOSS_FALL_STATE) &&
       Odin::mainOdin->getState() != ODIN_KNOCKBACK_STATE &&
       Odin::mainOdin->getState() != ODIN_AWAKE_FROM_KNOCKBACK_STATE &&
       Odin::mainOdin->getState() != ODIN_KNOCKBACK_EXPLOSION_STATE &&
       Odin::mainOdin->hitBoxIntersects(this))
    {
        Sounds::arrowHit[getRand(0,7)]->play2d();
        Odin::mainOdin->damage(ARROW_DAMAGE);
        getParent()->removeChild(this);
    }
}