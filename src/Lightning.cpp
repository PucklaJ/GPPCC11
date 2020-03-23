#include "../include/Lightning.h"
#include "../include/Defines.h"
#include "../include/Player.h"
#include "../include/Game.h"
#include <vector>
#include "../include/Sounds.h"
#include <Johnny.h>

Texture* Lightning::m_lightning = nullptr;
Texture* Lightning::m_lightning1 = nullptr;
Texture* Lightning::m_lightning2 = nullptr;
Texture* Lightning::m_lightning_big = nullptr;
Texture* Lightning::m_lightning_big1 = nullptr;
Texture* Lightning::m_lightning_big2 = nullptr;

Lightning::Lightning(const Vector2f& position) : Sprite2D(nullptr)
{
    setPosition(position);
    setName("Lightning");
}

Lightning::~Lightning()
{
    
}

bool Lightning::init()
{
    Sprite2D::init();
    
    if(!m_lightning)
    {
        m_lightning = m_mainClass->getResourceManager()->loadTexture("lightning.png",Settings::geti(SPRITE2D_FILTERING));
    }
    if(!m_lightning1)
    {
        m_lightning1 = m_mainClass->getResourceManager()->loadTexture("lightning1.png",Settings::geti(SPRITE2D_FILTERING));
    }
    if(!m_lightning2)
    {
        m_lightning2 = m_mainClass->getResourceManager()->loadTexture("lightning2.png",Settings::geti(SPRITE2D_FILTERING));
    }
    if(!m_lightning_big)
    {
        m_lightning_big = m_mainClass->getResourceManager()->loadTexture("lightning_big.png",Settings::geti(SPRITE2D_FILTERING));
    }
    if(!m_lightning_big1)
    {
        m_lightning_big1 = m_mainClass->getResourceManager()->loadTexture("lightning_big1.png",Settings::geti(SPRITE2D_FILTERING));
    }
    if(!m_lightning_big2)
    {
        m_lightning_big2 = m_mainClass->getResourceManager()->loadTexture("lightning_big2.png",Settings::geti(SPRITE2D_FILTERING));
    }
    
    m_velocity.x = -LIGHTNING_VELOCITY;
    
    setTexture(m_lightning);
    setScale(GAME_SCALE,GAME_SCALE);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    setDepth(LIGHTNING_DEPTH);
    
    std::vector<Texture*> animation;
    animation.push_back(m_lightning);
    animation.push_back(m_lightning1);
    animation.push_back(m_lightning2);
    
    addTween(new AnimationTween2D(animation,0.05f,true));
    
    return true;
}

bool Lightning::update()
{
    Sprite2D::update();
    
    m_time += m_mainClass->getTimer()->getDeltaTimeInSeconds();
    if(!m_big)
        m_time = 0.0f;
    
    controlPhysics();
    if(m_big)
        controlHitPlayer();
    else
        checkGettingBig();
        
    if(m_big)
    {
        if(m_time >= LIGHTNING_BE_BIG_TIME)
        {
            Sounds::lightning->stop();
            getParent()->removeChild(this);
        }
        else if(Player::mainPlayer && Player::mainPlayer->getState() != PLAYER_KNOCKBACK_STATE && intersects(Player::mainPlayer->getHitbox()))
        {
            Vector2f vel = Player::mainPlayer->getPosition() - getPosition();
            vel.normalise();
            Player::mainPlayer->hitByLightning(vel,ODIN_LIGHTNING_DAMAGE);
        }
    }
    else
    {
        if(Player::mainPlayer && Player::mainPlayer->getState() != PLAYER_KNOCKBACK_STATE && intersects(Player::mainPlayer->getHitbox()))
        {
            Vector2f vel = Player::mainPlayer->getPosition() - getPosition();
            vel.normalise();
            Player::mainPlayer->hitByLightning(vel,ODIN_LIGHTNING_SMALL_DAMAGE);
        }
    }
    
    return true;
}

void Lightning::quit()
{
    Sprite2D::quit();
}

void Lightning::controlPhysics()
{
    if(!m_big)
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
    
    if(!m_big)
    {
        if(getPosition().x < -SPEER_X_POS_DISAPPEAR || getPosition().x > m_mainClass->getNativeRes().width + SPEER_X_POS_DISAPPEAR ||
           getPosition().y < -SPEER_Y_POS_DISAPPEAR || getPosition().y > m_mainClass->getNativeRes().height + SPEER_Y_POS_DISAPPEAR)
        {
            getParent()->removeChild(this);
        }
    }
    
}

void Lightning::controlHitPlayer()
{
    
}

void Lightning::checkGettingBig()
{
    if(getPosition().x <= LIGHTNING_GET_BIG_X_POS)
    {
        setPosition(getPosition() + getActualSize()/2.0f - Vector2f(LIGHTNING_BIG_WIDTH,LIGHTNING_BIG_HEIGHT) / 2.0f);
        m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
        
        clearTweens();
        
        std::vector<Texture*> animation;
        animation.push_back(m_lightning_big);
        animation.push_back(m_lightning_big1);
        animation.push_back(m_lightning_big2);
    
        addTween(new AnimationTween2D(animation,0.05f,true));
        
        m_big = true;
        
        Sounds::lightning_throw[0]->stop();
        Sounds::lightning_throw[1]->stop();
        
        Sounds::lightning->play2d(true);
    }
}