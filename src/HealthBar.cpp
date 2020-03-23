#include "../include/HealthBar.h"
#include "../include/Defines.h"
#include <Johnny.h>

HealthBar::HealthBar(const std::string& fullFile,const std::string& halfFile,const Vector2f& position,bool direction,float maxHealth) : Actor("HealthBar"),
    m_health(maxHealth),
    m_maxHealth(maxHealth),
    m_direction(direction),
    m_position(position),
    m_fullFile(fullFile),
    m_halfFile(halfFile)
{
}

HealthBar::~HealthBar()
{
    
}

bool HealthBar::init()
{
    m_hearts = new Sprite2D*[(int)m_health];
    m_fullTexture = m_mainClass->getResourceManager()->loadTexture(m_fullFile,Settings::geti(SPRITE2D_FILTERING));
    m_halfTexture = m_mainClass->getResourceManager()->loadTexture(m_halfFile,Settings::geti(SPRITE2D_FILTERING));
    
    for(int i = 0;i<(int)m_health;i++)
    {
        m_hearts[i] = new Sprite2D(m_fullTexture);
        addChild(m_hearts[i]);
        m_hearts[i]->setPosition(m_direction == HEALTH_BAR_LEFT ? (m_position+Vector2f(HEALTH_BAR_PADDING,HEALTH_BAR_PADDING)+(Vector2f(HEALTH_BAR_HEART_WIDTH+HEALTH_BAR_PADDING,0.0f)*(GLfloat)i)) : (m_position-Vector2f(HEALTH_BAR_HEART_WIDTH+HEALTH_BAR_PADDING,-HEALTH_BAR_PADDING)+(Vector2f(-HEALTH_BAR_HEART_WIDTH-HEALTH_BAR_PADDING,0.0f)*(GLfloat)i)));
        m_hearts[i]->setScale(GAME_SCALE,GAME_SCALE);
        m_hearts[i]->setDepth(HUD_DEPTH);
    }
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    
    
    return true;
}

bool HealthBar::update()
{
    return true;
}

void HealthBar::quit()
{
    delete[] m_hearts;
}

void HealthBar::reduce(float amount)
{
    m_health -= amount;
    if(m_health < 0.0f)
    {
        m_health = 0.0f;
    }
    
    for(int i = 0;i<(int)m_maxHealth;i++)
    {
        if((int)(10.0f*m_health) < i*10+5)
        {
            m_hearts[i]->setVisible(false);
        }
        else
        {
            m_hearts[i]->setVisible(true);
            m_hearts[i]->setTexture(m_fullTexture);
            m_hearts[i]->setPosition(m_direction == HEALTH_BAR_LEFT ? (m_position+Vector2f(HEALTH_BAR_PADDING,HEALTH_BAR_PADDING)+(Vector2f(HEALTH_BAR_HEART_WIDTH+HEALTH_BAR_PADDING,0.0f)*(GLfloat)i)) : (m_position-Vector2f(HEALTH_BAR_HEART_WIDTH+HEALTH_BAR_PADDING,-HEALTH_BAR_PADDING)+(Vector2f(-HEALTH_BAR_HEART_WIDTH-HEALTH_BAR_PADDING,0.0f)*(GLfloat)i)));
        }
    }
    
    
    if(((int)(10.0f*m_health)) % 2 != 0)
    {
        getHeartForHealth(m_health)->setTexture(m_halfTexture);
        
        if(m_direction == HEALTH_BAR_RIGHT)
        {
            int index = (int)ceil((double)m_health)-1;
            getHeartForHealth(m_health)->setPosition(m_position-Vector2f(HEALTH_BAR_HEART_WIDTH+HEALTH_BAR_PADDING,-HEALTH_BAR_PADDING)+(Vector2f(-HEALTH_BAR_HEART_WIDTH-HEALTH_BAR_PADDING,0.0f)*(GLfloat)index)+Vector2f(HEALTH_BAR_HEART_WIDTH-HEALTH_BAR_HEART_HALF_WIDTH,0.0f));
        }
    }
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
}

Sprite2D* HealthBar::getHeartForHealth(float health)
{
    return m_hearts[(int)ceil((double)health)-1];
}