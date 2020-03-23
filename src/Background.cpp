#include "../include/Background.h"
#include "../include/Defines.h"
#include "../include/Game.h"
#include <Johnny.h>

Background::Background() : Actor("Background")
{
    
}

Background::~Background()
{
    
}

bool Background::init()
{
    Actor::init();
    
    for(int i = 0;i<3;i++)
        m_backgroundIndex[i] = -1;
    
    m_backgroundTex[0] = m_mainClass->getResourceManager()->loadTexture("Background1.png",Settings::geti(SPRITE2D_FILTERING));
    m_backgroundTex[1] = m_mainClass->getResourceManager()->loadTexture("Background2.png",Settings::geti(SPRITE2D_FILTERING));
    m_backgroundTex[2] = m_mainClass->getResourceManager()->loadTexture("Background3.png",Settings::geti(SPRITE2D_FILTERING));
    
    for(int i = 0;i<3;i++)
    {
        short random = getRandomBackgroundIndex(i);
        m_backgrounds[i] = new Sprite2D(m_backgroundTex[random]);
        m_backgroundIndex[i] = random;
        m_backgrounds[i]->setDepth(BACKGROUND_DEPTH);
        addChild(m_backgrounds[i]);
        m_backgrounds[i]->setScale(GAME_SCALE,GAME_SCALE);
        m_backgrounds[i]->setPosition(BACKGROUND_X_POS,i*m_backgroundTex[0]->getHeight()*GAME_SCALE);
    }
        
    
    return true;
}

bool Background::update()
{
    for(int i = 0;i<3;i++)
    {
        if(m_backgrounds[i]->getPosition().y <= -m_backgrounds[i]->getActualSize().height)
        {
            setNewBackground();
        }
        
        if(m_scrolling)
            m_backgrounds[i]->addPosition(0.0f,-BACKGROUND_SCROLL_VELOCITY * m_mainClass->getTimer()->getDeltaTimeInSeconds());
    }
    
    return true;
}

void Background::quit()
{
    Actor::quit();
}

void Background::startScrolling()
{
    m_scrolling = true;
}

void Background::stopScrolling()
{
    m_scrolling = false;
}

void Background::setNewBackground()
{
    short random = getRandomBackgroundIndex(m_middleBackground);
    m_backgrounds[m_middleBackground]->setPosition(m_backgrounds[m_middleBackground == 0 ? 2 : (m_middleBackground == 1 ? 0 : 1)]->getPosition() + Vector2f(0.0f,m_backgroundTex[0]->getHeight()*GAME_SCALE));
    m_backgrounds[m_middleBackground]->setTexture(m_backgroundTex[random]);
    m_backgroundIndex[m_middleBackground] = random;
    
    if(++m_middleBackground > 2)
    {
        m_middleBackground = 0;
    }
    
    if(random == 2)
        m_canSpawnObstacles = false;
    else
        m_canSpawnObstacles = true;
}

short Background::getRandomBackgroundIndex(short notcheck)
{
    short num = 0;
    
    do
    {
        num = (short)getRand(0,luck(10) ? 2 : 1);
    }while(!((notcheck == 0 || num != m_backgroundIndex[0]) &&
           (notcheck == 1 || num != m_backgroundIndex[1]) &&
           (notcheck == 2 || num != m_backgroundIndex[2])));

    return num;
}