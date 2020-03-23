#ifndef PLAYER_H
#define PLAYER_H

#include <Sprite2D.h>
#include <JoystickListener.h>
#include <vector>
#include "Defines.h"
#include <TextActor2D.h>
#include "Animations.h"

using namespace Johnny;

class Odin;
class Speer;
class HealthBar;

class Player : public Sprite2D, public AxisButtonListener
{
public:
    friend class Odin;
    friend class Speer;

    static Player* mainPlayer;
    static Vector2f gravity;

    Player();
    ~Player();
    
    bool init() override;
    bool update() override;
    bool render() override;
    void quit() override;
    
    void onAxisMotion(const ControllerAxisEvent &e) override;
    void onButtonDown(const ControllerButtonEvent &e) override;
    void onButtonUp(const ControllerButtonEvent &e) override;
    
    void hitBySpeer(const Vector2f& position);
    void hitByHit(const Vector2f& position);
    void hitByLightning(const Vector2f& velocity,float dmg);
    void hitByObstacle(const Vector2f& velocity);
    
    void collectArrowPickup();
    
    bool transitionDone = false;
    bool introDone = false;
    
    const int getState() const {return m_state;}
    float getHealth() const {return m_health;}
    
    const Vector2f& getOffset() const {return m_offset;}
    Johnny::Rectangle<GLfloat> getHitbox() const;
    const Johnny::Rectangle<GLfloat>& getSwordHitox() const {return m_swordHitBox;}
private:
    void processInput();
    void movement();
    void createBody();
    void controlPhysics();
    void damage(const Vector2f& velocity,float dmg);
    void setFlip(Flip flip);
    
    void loadAnimations();
    void setAnimation(Animation anim);
    Animation m_currentAnim = Animation::PLAYER_STAND;
    
    bool isPressed(int i);
    bool justPressed(int i);

    bool m_pressedActions[BUTTON_SIZE];
    bool m_previousPressedActions[BUTTON_SIZE];
    bool m_pressedControllerButtons[BUTTON_SIZE];
    int m_xAxis = 0;
    int m_yAxis = 0;
    bool m_canJump = false;
    bool m_jump_up_started = false;
    bool m_jump_down_started = false;
    Johnny::Rectangle<GLfloat> m_swordHitBox;
    int m_lookDirection = ODIN_DIRECTION_STAND;
    int m_fallDirection = ODIN_DIRECTION_STAND;
    int m_hitDirection = ODIN_DIRECTION_RIGHT;
    
    int m_state = PLAYER_MOVE_STATE;
    float m_time = 0.0f;
    float m_time1 = 0.0f;
    int m_blinkID = 0;
    float m_health = PLAYER_HEALTH;
    TextActor2D* m_arrowText = nullptr;
    HealthBar* m_healthBar = nullptr;
    HealthBar* m_arrowBar = nullptr;
    
    int m_numArrows = PLAYER_MAX_ARROWS;
    
    Vector2f m_velocity;
    Texture* m_sheet = nullptr;
    bool m_isWalking = false;
    
    Vector2f m_offset;
};

#endif