#ifndef ODIN_H
#define ODIN_H

#include <Sprite2D.h>
#include "Defines.h"
#include <TextActor2D.h>
#include "Animations.h"

using namespace Johnny;

class Player;
class BossfightScreen;
class HealthBar;

class Odin : public Sprite2D
{
public:
    friend class Player;
    friend class BossfightScreen;

    static Odin* mainOdin;

    Odin();
    ~Odin();
    
    bool init() override;
    bool update() override;
    bool render() override;
    void quit() override;
    
    void damage(float dmg);
    
    bool transitionDone = false;
    bool introDone = false;
    
    int getState() const {return m_state;}
    
    bool hitBoxIntersects(Sprite2D* spr);
    bool hitBoxIntersects(const Johnny::Rectangle<GLfloat>& rect);
    
    bool hitHitBoxIntersects(const Johnny::Rectangle<GLfloat>& rect);
    Johnny::Rectangle<GLfloat> getHitHitbox();
    
private:
    void updateKI();
    void controlPhysics();
    void throwSpeer();
    void throwLightning();
    void hitOnGround();
    void setFlip(Flip flip);
    
    void loadAnimations();
    void setAnimation(Animation anim);
    
    HealthBar* m_healthBar = nullptr;
    
    float m_time = 0.0f;
    float m_time1 = 0.0f; 
    float m_health = ODIN_GROUND_HEALTH;
    
    Vector2f m_velocity;
    int m_state = 0;
    int m_walkDirection;
    int m_thrownSpeers = 0;
    int m_hitDirection;
    int m_blinkID = 0;
    int m_fallWalkID = 0;
    float m_fallToWalkTime = 1.0f;
    int m_previousLightningPos = ODIN_DIRECTION_UP;
    
    Animation m_currentAnim = ODIN_RUN;
    
    Vector2f m_offset;
    
    Texture* m_stand = nullptr;
    Texture* m_run1 = nullptr;
    Texture* m_run15 = nullptr;
    Texture* m_run2 = nullptr;
    Texture* m_run25 = nullptr;
    Texture* m_fall = nullptr;
    Texture* m_fall1 = nullptr;
    Texture* m_fall15 = nullptr;
    Texture* m_fall2 = nullptr;
    Texture* m_fall25 = nullptr;
    Texture* m_speer = nullptr;
    Texture* m_speer1 = nullptr;
    Texture* m_speer2 = nullptr;
    Texture* m_speer_fall = nullptr;
    Texture* m_speer_fall1 = nullptr;
    Texture* m_speer_fall2 = nullptr;
    Texture* m_hit = nullptr;
    Texture* m_hit1 = nullptr;
    Texture* m_hit2 = nullptr;
    Texture* m_hit3 = nullptr;
    Texture* m_lightning1 = nullptr;
    Texture* m_lightning2 = nullptr;
    Texture* m_knockback = nullptr;
    Texture* m_knockback_explosion = nullptr;
    
    float m_time2 = 0.0f;
};

#endif