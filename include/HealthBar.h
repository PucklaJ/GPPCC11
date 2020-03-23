#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H
#include <Actor.h>
#include <string>
#include <Vector2.h>

namespace Johnny
{
    class Sprite2D;
    class Texture;
}

using namespace Johnny;

class HealthBar : public Actor
{
public:
    HealthBar(const std::string& fullFile,const std::string& halfFile,const Vector2f& position,bool direction,float maxHealth);
    ~HealthBar();
    
    bool init() override;
    bool update() override;
    void quit() override;
    
    void reduce(float amount);
    void setMaxHealth(float health) {m_maxHealth = health;}
    
    float getHealth() const {return m_health;}
    
private:
    Sprite2D* getHeartForHealth(float health);

    Texture* m_fullTexture = nullptr;
    Texture* m_halfTexture = nullptr;

    Sprite2D** m_hearts = nullptr;
    float m_health = 0.0f;
    float m_maxHealth = 0.0f;
    bool m_direction = false;
    Vector2f m_position;
    std::string m_fullFile = "";
    std::string m_halfFile = "";
};


#endif