#include "../include/Player.h"
#include "../include/Odin.h"
#include "../include/Game.h"
#include "../include/Arrow.h"
#include "../include/BossfightScreen.h"
#include "../include/Regions.h"
#include "../include/Background.h"
#include "../include/HealthBar.h"
#include "../include/Sounds.h"
#include <Johnny.h>

Player* Player::mainPlayer = nullptr;
Vector2f Player::gravity(0.0f,500.0f);

Player::Player() : Sprite2D("player_sheet.png")
{
    for(int i = 0;i<BUTTON_SIZE;i++)
    {
        m_pressedActions[i] = false;
        m_pressedControllerButtons[i] = false;
        m_previousPressedActions[i] = false;
    }
    
    mainPlayer = this;
    setName("Player");
}

Player::~Player()
{
    
}

bool Player::init()
{
    Sprite2D::init();
    
    setScale(GAME_SCALE,GAME_SCALE);
    
    createBody();
    
    m_mainClass->getJoystickManager()->getListener()->addListener(this,0);
    
    m_swordHitBox.width = PLAYER_HITBOX_WIDTH;
    m_swordHitBox.height = PLAYER_HITBOX_HEIGHT;
    
    setDepth(PLAYER_DEPTH);
    
    loadAnimations();
    
    setTexture(m_sheet);
    setSrcRegion(Regions::Player::Ground::Stand::stand);
    setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
    
    setPosition(m_mainClass->getNativeRes()/2.0f - Vector2f(PLAYER_WIDTH,PLAYER_HEIGHT)/2.0f);
    
    m_healthBar = new HealthBar("heart.png","heart1.png",Vector2f(0.0f,0.0f),HEALTH_BAR_LEFT,PLAYER_HEALTH);
    m_arrowBar = new HealthBar("arrowBar.png","arrowBar.png",Vector2f(0.0f,HEALTH_BAR_HEART_WIDTH + HEALTH_BAR_PADDING + HEALTH_BAR_PADDING),HEALTH_BAR_LEFT,10.0f);
    
    addChild(m_healthBar);
    addChild(m_arrowBar);
    
    m_arrowBar->reduce(10.0f-(float)m_numArrows);
      
    return true;
}

bool Player::update()
{
    if(Game::state == GAME_GAME_OVER_STATE)
        return true;
    
    processInput();
    movement();
    controlPhysics();
    
    if(JOHNNY_INPUT->justPressed(Keys::Z))
    {
        Odin::mainOdin->damage(4.0f);
    }
    
    setFlip(m_lookDirection == ODIN_DIRECTION_RIGHT ? Flip::NONE : Flip::HORIZONTALLY);
    
    Sprite2D::update();
    
    return true;
}

bool Player::render()
{
    setPosition(getPosition() + m_offset);
    
    Sprite2D::render();
    
    setPosition(getPosition() - m_offset);
    
    return true;
}

void Player::quit()
{
    Sprite2D::quit();
}

void Player::onAxisMotion(const ControllerAxisEvent &e)
{
    if(e.axis == Axis::LEFT_STICK_X)
    {
        m_xAxis = e.value;
    }
    else if(e.axis == Axis::LEFT_STICK_Y)
    {
        m_yAxis = e.value;
    }
}

void Player::onButtonDown(const ControllerButtonEvent &e)
{
    if(e.button == Buttons::LEFT)
        m_pressedControllerButtons[LEFT_KEY] = true;
    else if(e.button == Buttons::RIGHT)
        m_pressedControllerButtons[RIGHT_KEY] = true;
    else if(e.button == Buttons::CROSS)
        m_pressedControllerButtons[JUMP_KEY] = true;
    else if(e.button == Buttons::SQUARE)
        m_pressedControllerButtons[SWORD_KEY] = true;
    else if(e.button == Buttons::CIRCLE)
        m_pressedControllerButtons[BOW_KEY] = true;
    else if(e.button == Buttons::UP)
        m_pressedControllerButtons[UP_KEY] = true;
    else if(e.button == Buttons::DOWN)
        m_pressedControllerButtons[DOWN_KEY] = true;
}

void Player::onButtonUp(const ControllerButtonEvent &e)
{
     if(e.button == Buttons::LEFT)
        m_pressedControllerButtons[LEFT_KEY] = false;
    else if(e.button == Buttons::RIGHT)
        m_pressedControllerButtons[RIGHT_KEY] = false;
    else if(e.button == Buttons::CROSS)
        m_pressedControllerButtons[JUMP_KEY] = false;
    else if(e.button == Buttons::SQUARE)
        m_pressedControllerButtons[SWORD_KEY] = false;
    else if(e.button == Buttons::CIRCLE)
        m_pressedControllerButtons[BOW_KEY] = false;
    else if(e.button == Buttons::UP)
        m_pressedControllerButtons[UP_KEY] = false;
    else if(e.button == Buttons::DOWN)
        m_pressedControllerButtons[DOWN_KEY] = false;
}

void Player::processInput()
{
    for(int i = 0;i<BUTTON_SIZE;i++)
        m_previousPressedActions[i] = m_pressedActions[i];
    
    if(abs(m_xAxis) < 10000)
        m_xAxis = 0;
    if(abs(m_yAxis) < 10000)
        m_yAxis = 0;
    
    m_pressedActions[LEFT_KEY] = (m_pressedControllerButtons[LEFT_KEY] || m_xAxis < 0) || JOHNNY_INPUT->isPressed(Keys::A);
    m_pressedActions[RIGHT_KEY] = (m_pressedControllerButtons[RIGHT_KEY] || m_xAxis > 0) || JOHNNY_INPUT->isPressed(Keys::D);
    m_pressedActions[UP_KEY] = (m_pressedControllerButtons[UP_KEY] || m_yAxis < 0) || JOHNNY_INPUT->isPressed(Keys::W);
    m_pressedActions[DOWN_KEY] = (m_pressedControllerButtons[DOWN_KEY] || m_yAxis > 0) || JOHNNY_INPUT->isPressed(Keys::S);
    m_pressedActions[SWORD_KEY] = m_pressedControllerButtons[SWORD_KEY] || JOHNNY_INPUT->isPressed(Keys::J);
    m_pressedActions[BOW_KEY] = m_pressedControllerButtons[BOW_KEY] || JOHNNY_INPUT->isPressed(Keys::K);
    m_pressedActions[JUMP_KEY] = m_pressedControllerButtons[JUMP_KEY] || JOHNNY_INPUT->isPressed(Keys::SPACE);
}

void Player::movement()
{
    m_time += JOHNNY_DT;
    
    if((Game::state == GAME_BOSS_GROUND_STATE || Game::state == GAME_BOSS_FALL_STATE) && 
       Odin::mainOdin->m_state != ODIN_KNOCKBACK_STATE &&
       m_state != PLAYER_KNOCKBACK_STATE &&
       Odin::mainOdin->hitBoxIntersects(getHitbox()))
    {
        Vector2f pos = (Odin::mainOdin->getPosition() + Odin::mainOdin->getActualSize()/2.0f - getPosition()) * -1.0f;
        pos.normalise();
        damage(pos,ODIN_TOUCH_DAMAGE);
    }
    
    if(m_state == PLAYER_MOVE_STATE)
    {
        Vector2f velocity = m_velocity;
        Vector2f velocityToSet = velocity;
        
        if(Game::state != GAME_BOSS_FALL_STATE &&
           Game::state != GAME_FALL_GROUND_TRANSITION_STATE &&
           Game::state != GAME_INTRO_STATE &&
           Game::state != GAME_FALL_WIN_TRANSITION_STATE)
        {
            if(isPressed(RIGHT_KEY))
            {
                if(velocityToSet.x < (m_canJump ? PLAYER_VELOCITY : (PLAYER_VELOCITY / PLAYER_VELOCITY_AIR_DIVIDER)))
                    velocityToSet.x += (m_canJump ? PLAYER_ACCELARATION : (PLAYER_ACCELARATION / PLAYER_VELOCITY_AIR_DIVIDER));
                
                if(Game::state != GAME_FALL_INTRO_STATE && Game::state != GAME_GROUND_INTRO_STATE &&
                   m_canJump)
                {
                    setAnimation(Animation::PLAYER_RUN);
                }
                
                m_lookDirection = ODIN_DIRECTION_RIGHT;
                m_hitDirection = ODIN_DIRECTION_RIGHT;
                if(m_canJump)
                    m_isWalking = true;
            }
            else if(isPressed(LEFT_KEY))
            {
                if(velocityToSet.x > -(m_canJump ? PLAYER_VELOCITY : (PLAYER_VELOCITY / PLAYER_VELOCITY_AIR_DIVIDER)))
                    velocityToSet.x -= (m_canJump ? PLAYER_ACCELARATION : (PLAYER_ACCELARATION / PLAYER_VELOCITY_AIR_DIVIDER));
                    
                
                if(Game::state != GAME_FALL_INTRO_STATE && Game::state != GAME_GROUND_INTRO_STATE &&
                   m_canJump)
                {
                    setAnimation(Animation::PLAYER_RUN);
                }
                
                
                m_lookDirection = ODIN_DIRECTION_LEFT;
                m_hitDirection = ODIN_DIRECTION_LEFT;
                if(m_canJump)
                    m_isWalking = true;
            }
            else
            {
                if(abs(velocityToSet.x) > 0.0f)
                {
                    if(abs(velocityToSet.x) <= 10.0f)
                        velocityToSet.x = 0.0f;
                    else
                    {
                        if(velocityToSet.x < 0.0f)
                            velocityToSet.x += PLAYER_VELOCITY / PLAYER_DAMPING_DIVIDER;
                        else
                            velocityToSet.x -= PLAYER_VELOCITY / PLAYER_DAMPING_DIVIDER;
                    }
                }
                
                if(Game::state != GAME_FALL_INTRO_STATE && m_canJump)
                {
                    setAnimation(Animation::PLAYER_STAND);
                }
                
                m_isWalking = false;
                
            }
            
            if(m_canJump && justPressed(JUMP_KEY))
            {
                velocityToSet += Vector2f(0.0f,-PLAYER_JUMP_POWER);
                m_canJump = false;
                m_isWalking = false;
                Sounds::jump[getRand(0,3)]->play2d();
            }
            
            if(!m_canJump ||
               Game::state == GAME_GROUND_FALL_TRANSITION_STATE ||
               Game::state == GAME_GROUND_INTRO_STATE ||
               Game::state == GAME_GROUND_WIN_INTRO_STATE)
            {
                if(Game::state != GAME_FALL_INTRO_STATE)
                {
                    if(m_velocity.y < 0.0f)
                    {
                        setAnimation(Animation::PLAYER_JUMP_UP);
                    }
                    else if(m_velocity.y > 0.0f)
                    {
                        setAnimation(Animation::PLAYER_JUMP_DOWN);
                    }
                }
            }
            
            if(m_isWalking)
            {
                m_time1 += m_mainClass->getTimer()->getDeltaTimeInSeconds();
                if(m_time1 >= 0.25f)
                {
                    if(Sounds::walkWood[0]->isPlaying())
                        Sounds::walkWood[1]->play2d();
                    else
                        Sounds::walkWood[1]->play2d();
                        
                    m_time1 = 0.0f;
                }
            }
        
        }
        else
        {
            if(isPressed(RIGHT_KEY))
            {
                if(velocityToSet.x < PLAYER_VELOCITY)
                    velocityToSet.x += PLAYER_ACCELARATION;
                
                
                setAnimation(PLAYER_FALL_RIGHT);
                
                m_lookDirection = ODIN_DIRECTION_RIGHT;
                m_hitDirection = ODIN_DIRECTION_RIGHT;
            }
            else if(isPressed(LEFT_KEY))
            {
                if(velocityToSet.x > -PLAYER_VELOCITY)
                    velocityToSet.x -= PLAYER_ACCELARATION;
                    
                setAnimation(Animation::PLAYER_FALL_LEFT);
                m_lookDirection = ODIN_DIRECTION_LEFT;
                m_hitDirection = ODIN_DIRECTION_LEFT;
            }
            else
            {
                if(abs(velocityToSet.x) > 0.0f)
                {
                    if(abs(velocityToSet.x) <= 10.0f)
                        velocityToSet.x = 0.0f;
                    else
                    {
                        if(velocityToSet.x < 0.0f)
                            velocityToSet.x += PLAYER_VELOCITY / PLAYER_DAMPING_DIVIDER;
                        else
                            velocityToSet.x -= PLAYER_VELOCITY / PLAYER_DAMPING_DIVIDER;
                    }
                }
                
               
                if(m_fallDirection != ODIN_DIRECTION_UP)
                    setAnimation(Animation::PLAYER_FALL_DOWN);
                //m_lookDirection = ODIN_DIRECTION_STAND;
            }
            
            if(Game::state == GAME_BOSS_FALL_STATE || Game::state == GAME_INTRO_STATE)
            {
                if(isPressed(DOWN_KEY))
                {
                    if(velocityToSet.y < PLAYER_VELOCITY)
                        velocityToSet.y += PLAYER_ACCELARATION;
                        
                    if(m_lookDirection == ODIN_DIRECTION_STAND)
                    {
                        setAnimation(Animation::PLAYER_FALL_DOWN);
                    }
                    
                        
                    m_fallDirection = ODIN_DIRECTION_DOWN;
                }
                else if(isPressed(UP_KEY))
                {
                    if(velocityToSet.y > -PLAYER_VELOCITY)
                        velocityToSet.y -= PLAYER_ACCELARATION;
                    
                    setAnimation(Animation::PLAYER_FALL_UP);
                    
                    
                    m_fallDirection = ODIN_DIRECTION_UP;
                }
                else
                {
                    if(abs(velocityToSet.y) > 0.0f)
                    {
                        if(abs(velocityToSet.y) <= 10.0f)
                            velocityToSet.y = 0.0f;
                        else
                        {
                            if(velocityToSet.y < 0.0f)
                                velocityToSet.y += PLAYER_VELOCITY / PLAYER_DAMPING_DIVIDER;
                            else
                                velocityToSet.y -= PLAYER_VELOCITY / PLAYER_DAMPING_DIVIDER;
                        }
                    }
                    
                    if(m_lookDirection == ODIN_DIRECTION_STAND)
                    {
                        setAnimation(Animation::PLAYER_FALL_DOWN);
                    }
                    
                    m_fallDirection = ODIN_DIRECTION_STAND;
                }
            }
            
        }
        
        m_velocity = velocityToSet;
        m_time = 0.0f;
        
        
        if(justPressed(SWORD_KEY))
        {
            m_state = PLAYER_SWORD_ATTACK_STATE;
            m_velocity.x /= PLAYER_SWORD_ATTACK_VELOCITY_DIVIDER;
            if(Game::state == GAME_BOSS_FALL_STATE || Game::state == GAME_FALL_GROUND_TRANSITION_STATE || Game::state == GAME_INTRO_STATE)
                m_velocity.y /= PLAYER_SWORD_ATTACK_VELOCITY_DIVIDER;
            if(Game::state == GAME_BOSS_GROUND_STATE ||
               Game::state == GAME_GROUND_FALL_TRANSITION_STATE ||
               Game::state == GAME_WIN_STATE ||
               Game::state == GAME_GROUND_INTRO_STATE)
                setAnimation(Animation::PLAYER_SWORD);
            else
                setAnimation(Animation::PLAYER_SWORD_FALL);
                
            Sounds::sword[getRand(0,1)]->play2d();
        }
        else if(m_numArrows > 0 && justPressed(BOW_KEY))
        {
            m_state = PLAYER_BOW_PREPARE_STATE;
            m_velocity.x /= PLAYER_BOW_ATTACK_VELOCITY_DIVIDER;
            if(Game::state == GAME_BOSS_FALL_STATE || Game::state == GAME_FALL_GROUND_TRANSITION_STATE || Game::state == GAME_INTRO_STATE)
                m_velocity.y /= PLAYER_BOW_ATTACK_VELOCITY_DIVIDER;
            
            setAnimation(Animation::PLAYER_BOW);
            
            Sounds::bow->play2d();
        }
        
    }
    else if(m_state == PLAYER_KNOCKBACK_STATE)
    {
        if(m_time >= PLAYER_KNOCKBACK_TIME)
        {
            m_time = 0.0f;
            m_state = PLAYER_MOVE_STATE;
            stopTween(m_blinkID);
        }
    }
    else if(m_state == PLAYER_SWORD_ATTACK_STATE)
    {
        Vector2f swordHitbox(getPosition() + (m_hitDirection == ODIN_DIRECTION_RIGHT ? Vector2f(PLAYER_WIDTH+PLAYER_HITBOX_X_POS,PLAYER_HITBOX_Y_POS) : Vector2f(PLAYER_HITBOX_X_POS - PLAYER_HITBOX_WIDTH,PLAYER_HITBOX_Y_POS)));
        
        m_swordHitBox.x = swordHitbox.x;
        m_swordHitBox.y = swordHitbox.y;
        
        if((Game::state == GAME_BOSS_GROUND_STATE || Game::state == GAME_BOSS_FALL_STATE) && 
           ((Odin::mainOdin->m_state != ODIN_KNOCKBACK_STATE &&
             Odin::mainOdin->m_state != ODIN_KNOCKBACK_EXPLOSION_STATE &&
             Odin::mainOdin->m_state != ODIN_AWAKE_FROM_KNOCKBACK_STATE &&
             Odin::mainOdin->m_state != ODIN_TELEPORT_ATTACK_STATE)
             && (Odin::mainOdin->hitBoxIntersects(m_swordHitBox))))
        {
            Odin::mainOdin->damage(PLAYER_SWORD_DAMAGE);
        }
        
        if(m_time >= PLAYER_SWORD_ATTACK_TIME)
        {
            m_time = 0.0f;
            m_state = PLAYER_MOVE_STATE;
        }
    }
    else if(m_state == PLAYER_BOW_PREPARE_STATE)
    {
        if(m_time >= PLAYER_BOW_PREPARE_TIME)
        {
            m_time = 0.0f;
            m_state = PLAYER_BOW_ATTACK_STATE;
            Arrow* arrow = new Arrow(getPosition() + (m_lookDirection == ODIN_DIRECTION_RIGHT ? Vector2f(10.0f,17.0f) : Vector2f(8.0f,17.0f)) * GAME_SCALE,m_hitDirection == ODIN_DIRECTION_LEFT ? Vector2f(-1.0f,0.0f) : Vector2f(1.0f,0.0f));
            addChild(arrow);
            m_numArrows--;
            m_arrowBar->reduce(1.0f);
            setAnimation(Animation::PLAYER_BOW_FINISH);
        }
    }
    else if(m_state == PLAYER_BOW_ATTACK_STATE)
    {
        if(m_time >= PLAYER_BOW_SHOOT_TIME)
        {
            m_time = 0.0f;
            m_state = PLAYER_MOVE_STATE;
        }
    }
    else if(m_state == PLAYER_INTRO_STATE)
    {
        setAnimation(PLAYER_FALL_DOWN);
        if(getPosition().y > m_mainClass->getNativeRes().height)
        {
            setPosition(getPosition().x,PLAYER_INTRO_Y_POS);
            m_state = PLAYER_MOVE_STATE;
            Game::state = GAME_BOSS_GROUND_STATE;
            setAnimation(PLAYER_JUMP_DOWN);
            BossfightScreen::mainBossfightScreen->addBoundaries();
            
            
            BossfightScreen::mainBossfightScreen->setOdin(new Odin());
            BossfightScreen::mainBossfightScreen->addChild(BossfightScreen::mainBossfightScreen->getOdin());
            BossfightScreen::mainBossfightScreen->getOdin()->setPosition(BossfightScreen::mainBossfightScreen->getOdin()->getPosition().x,m_mainClass->getNativeRes().height);
            BossfightScreen::mainBossfightScreen->getGround()->setPosition(GROUND_POS_X,m_mainClass->getNativeRes().height + ODIN_HEIGHT);
            
            BossfightScreen::mainBossfightScreen->getGround()->addTween(new PositionTween2D(Vector2f(GROUND_POS_X,_GROUND_POS_Y_(true)),1.0f));
            BossfightScreen::mainBossfightScreen->getOdin()->addTween(new PositionTween2D(Vector2f(BossfightScreen::mainBossfightScreen->getOdin()->getPosition().x,m_mainClass->getNativeRes().height-GROUND_HEIGHT-ODIN_HEIGHT),1.0f));
        
            BossfightScreen::mainBossfightScreen->getOdin()->m_state = ODIN_INTRO_STATE;
        }
    }
    
    if(Game::state == GAME_INTRO_STATE)
    {
        if(m_time1 >= GAME_INTRO_TIME)
        {
            m_time1 = 0.0f;
            m_state = PLAYER_INTRO_STATE;
            m_velocity = Vector2f(0.0f,0.0f);
            Sounds::introTheme->stop();
            Sounds::mainTheme->play2d(true);
        }
        
        m_time1 += m_mainClass->getTimer()->getDeltaTimeInSeconds();
    }
}

void Player::createBody()
{
    
}

bool Player::isPressed(int i)
{
    return m_pressedActions[i];
}

bool Player::justPressed(int i)
{
    return !m_previousPressedActions[i] && m_pressedActions[i];
}

void Player::controlPhysics()
{
    if(Game::state == GAME_FALL_INTRO_STATE)
    {
        if(getPosition().y < PLAYER_FALL_INTRO_Y_END_POS)
        {
            m_velocity = Player::gravity;
            addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        }
        else
        {
            setPosition(getPosition().x,PLAYER_FALL_INTRO_Y_END_POS);
            introDone = true;
            m_velocity = Vector2f(0.0f,0.0f);
        }
        
    }
    else
    {
        if(m_state == PLAYER_INTRO_STATE || 
           (Game::state != GAME_BOSS_FALL_STATE &&
            Game::state != GAME_GROUND_INTRO_STATE &&
            Game::state != GAME_INTRO_STATE &&
            Game::state != GAME_GROUND_WIN_INTRO_STATE))
        {
            m_velocity += gravity * m_mainClass->getTimer()->getDeltaTimeInSeconds();
        }
        else if(Game::state == GAME_GROUND_INTRO_STATE ||
                Game::state == GAME_GROUND_WIN_INTRO_STATE)
            m_velocity = gravity;
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        
        if(Game::state != GAME_BOSS_FALL_STATE && 
           Game::state != GAME_GROUND_FALL_TRANSITION_STATE && 
           Game::state != GAME_FALL_GROUND_TRANSITION_STATE &&
           Game::state != GAME_INTRO_STATE &&
           Game::state != GAME_FALL_WIN_TRANSITION_STATE &&
           (getPosition().y + PLAYER_HEIGHT > GROUND_POS_Y))
        {
            setPosition(Vector2f(getPosition().x,GROUND_POS_Y) - Vector2f(0.0f,PLAYER_HEIGHT));
            m_velocity.y = 0.0f;
            
            if(!m_canJump)
            {
                cAudio::IAudioSource* sound = Sounds::walkWood[getRand(0,1)];
                sound->setVolume(DEFAULT_VOLUME);
                sound->play2d();
                sound->setVolume(DEFAULT_VOLUME/2.0f);
            }
                
            
            m_canJump = true;
            BossfightScreen::mainBossfightScreen->getBackground()->stopScrolling();
            
            
            if(Game::state == GAME_GROUND_INTRO_STATE || Game::state == GAME_GROUND_WIN_INTRO_STATE)
            {
                introDone = true;
            }
                
        }
        else if(Game::state != GAME_INTRO_STATE && Game::state != GAME_BOSS_FALL_STATE && getPosition().y < -PLAYER_Y_BORDER)
        {
            setPosition(Vector2f(getPosition().x,-PLAYER_Y_BORDER));
            m_velocity.y = 0.0f;
        }
        else if(Game::state != GAME_INTRO_STATE && Game::state == GAME_BOSS_FALL_STATE && getPosition().y < 0.0f)
        {
            setPosition(getPosition().x,0.0f);
            m_velocity.y = 0.0f;
        }
        
        if(m_state != PLAYER_INTRO_STATE &&
          ((Game::state == GAME_INTRO_STATE || Game::state == GAME_BOSS_FALL_STATE) &&
            getPosition().y + PLAYER_HEIGHT > m_mainClass->getNativeRes().height))
        {
            setPosition(getPosition().x,m_mainClass->getNativeRes().height-PLAYER_HEIGHT);
            m_velocity.y = 0.0f;
        }
        
        if(getPosition().x + PLAYER_WIDTH > m_mainClass->getNativeRes().x)
        {
            setPosition(m_mainClass->getNativeRes().x-PLAYER_WIDTH,getPosition().y);
            m_velocity.x = 0.0f;
        }
        else if(getPosition().x < 0.0f)
        {
            setPosition(0.0f,getPosition().y);
            m_velocity.x = 0.0f;
        }
        
        if(Game::state == GAME_FALL_GROUND_TRANSITION_STATE)
        {
            if(getPosition().y > PLAYER_FALL_GROUND_TRANSITION_END_Y_POS)
            {
                setPosition(getPosition().x,PLAYER_FALL_GROUND_TRANSITION_END_Y_POS);
                transitionDone = true;
                setAnimation(Animation::PLAYER_STAND);
            }
        }
        else if(Game::state == GAME_GROUND_FALL_TRANSITION_STATE)
        {
            if(getPosition().y > m_mainClass->getNativeRes().y + GAME_GROUND_FALL_TRANSITION_OVERLAP)
            {
                transitionDone = true;
                setAnimation(Animation::PLAYER_FALL_DOWN);
            }
        }
        else if(Game::state == GAME_FALL_WIN_TRANSITION_STATE)
        {
            if(getPosition().y > m_mainClass->getNativeRes().y + PLAYER_FALL_GROUND_TRANSITION_END_Y_POS)
            {
                transitionDone = true;
                setAnimation(Animation::PLAYER_STAND);
            }
        }
    }
    
}

void Player::hitBySpeer(const Vector2f& position)
{
    Vector2f pos = (Odin::mainOdin->getPosition() + Odin::mainOdin->getActualSize()/2.0f - getPosition()) * -1.0f;
    pos.normalise();
    damage(pos,ODIN_SPEER_DAMAGE);
}

void Player::hitByHit(const Vector2f& position)
{
    Vector2f pos = (position - getPosition()) * -1.0f;
    pos.normalise();
    
    damage(pos,ODIN_HIT_DAMAGE);
}

void Player::hitByLightning(const Vector2f& velocity,float dmg)
{
    damage(velocity * 0.0f,dmg);
}

void Player::hitByObstacle(const Vector2f& velocity)
{
    damage(velocity,OBSTACLE_DAMAGE);
}

void Player::damage(const Vector2f& velocity,float dmg)
{
    if(Game::state == GAME_BOSS_GROUND_STATE)
        setAnimation(Animation::PLAYER_KNOCKBACK);
    else
        setAnimation(Animation::PLAYER_KNOCKBACK_FALL);
    stopTween(m_blinkID);
    m_state = PLAYER_KNOCKBACK_STATE;
    m_velocity = velocity * PLAYER_KNOCKBACK_POWER;
    m_blinkID = addTween(new BlinkTween2D(0.1f));
    m_health -= dmg;
    m_healthBar->reduce(dmg);
    
    Sounds::bow->stop();
    Sounds::pain[getRand(0,5)]->play2d();
}

void Player::collectArrowPickup()
{
    if(m_numArrows == 10)
        return;
    
    Sounds::arrowPickUp->play2d();
    
    m_numArrows += PLAYER_ARROW_PICKUP_AMOUNT;
    m_arrowBar->reduce((float)-PLAYER_ARROW_PICKUP_AMOUNT);
}

void Player::setFlip(Flip flip)
{
    if(m_currentAnim != Animation::PLAYER_FALL_LEFT &&
       m_currentAnim != Animation::PLAYER_FALL_RIGHT &&
       m_currentAnim != Animation::PLAYER_FALL_DOWN && 
       m_currentAnim != Animation::PLAYER_FALL_UP)
        m_sheet->setFlip(flip);
    else
        m_sheet->setFlip(Flip::NONE);
}

void Player::loadAnimations()
{
    m_sheet = m_mainClass->getResourceManager()->loadTexture("player_sheet.png",Settings::geti(SPRITE2D_FILTERING));
}

void Player::setAnimation(Animation anim)
{
    if(m_currentAnim == anim)
        return;
        
    clearTweens();
    
    std::vector<TextureRegion> regions;
    
    m_offset = Vector2f(0.0f,0.0f);
    
    switch(anim)
    {
        case PLAYER_STAND:
            setTexture(m_sheet);
            setSrcRegion(Regions::Player::Ground::Stand::stand);
            setDrawSize((GLfloat)m_srcRegion.width,(GLfloat)m_srcRegion.height);
            break;
        case PLAYER_RUN:
            regions.push_back(Regions::Player::Ground::Run::run1);
            regions.push_back(Regions::Player::Ground::Run::run15);
            regions.push_back(Regions::Player::Ground::Stand::stand);
            regions.push_back(Regions::Player::Ground::Run::run2);
            regions.push_back(Regions::Player::Ground::Run::run25);
            regions.push_back(Regions::Player::Ground::Stand::stand);
            
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_JUMP_UP:
            regions.push_back(Regions::Player::Ground::Jump::Up::up1);
            regions.push_back(Regions::Player::Ground::Jump::Up::up2);
            regions.push_back(Regions::Player::Ground::Jump::Up::up1);
            regions.push_back(Regions::Player::Ground::Jump::Up::up3);
            
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_JUMP_DOWN:
            regions.push_back(Regions::Player::Ground::Jump::Down::down1);    
            regions.push_back(Regions::Player::Ground::Jump::Down::down2);
            regions.push_back(Regions::Player::Ground::Jump::Down::down1);
            regions.push_back(Regions::Player::Ground::Jump::Down::down3);    
            
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_FALL_RIGHT:
            regions.push_back(Regions::Player::Fall::Right::right1);
            regions.push_back(Regions::Player::Fall::Right::right2);
            regions.push_back(Regions::Player::Fall::Right::right1);
            regions.push_back(Regions::Player::Fall::Right::right3);
    
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_FALL_LEFT:
            regions.push_back(Regions::Player::Fall::Left::left1);        
            regions.push_back(Regions::Player::Fall::Left::left2);        
            regions.push_back(Regions::Player::Fall::Left::left1);        
            regions.push_back(Regions::Player::Fall::Left::left3);        
            
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_FALL_UP:
            regions.push_back(Regions::Player::Fall::Up::up1);        
            regions.push_back(Regions::Player::Fall::Up::up2);        
            regions.push_back(Regions::Player::Fall::Up::up1);        
            regions.push_back(Regions::Player::Fall::Up::up3);        
            
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_FALL_DOWN:
            regions.push_back(Regions::Player::Fall::Down::down1);
            regions.push_back(Regions::Player::Fall::Down::down2);
            regions.push_back(Regions::Player::Fall::Down::down1);
            regions.push_back(Regions::Player::Fall::Down::down3);
            
            addTween(new AnimationTween2D(regions,0.1f,true));
            setDrawSize(PLAYER_WIDTH/GAME_SCALE,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_SWORD:
            regions.push_back(Regions::Player::Ground::Sword::sword1);        
            regions.push_back(Regions::Player::Ground::Sword::sword2);        
            regions.push_back(Regions::Player::Ground::Sword::sword3);        
            
            addTween(new AnimationTween2D(regions,0.05f,false));
            if(m_hitDirection == ODIN_DIRECTION_LEFT)
                m_offset = Vector2f(-18.0f,0.0f) * GAME_SCALE;
            setDrawSize(PLAYER_WIDTH/GAME_SCALE+18.0f,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_SWORD_FALL:
            regions.push_back(Regions::Player::Fall::Sword::sword1);        
            regions.push_back(Regions::Player::Fall::Sword::sword2);        
            regions.push_back(Regions::Player::Fall::Sword::sword3);        
            
            addTween(new AnimationTween2D(regions,0.05f,false));
            if(m_lookDirection == ODIN_DIRECTION_LEFT)
                m_offset = Vector2f(-18.0f,0.0f) * GAME_SCALE;
            setDrawSize(PLAYER_WIDTH/GAME_SCALE+18.0f,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_BOW:
            regions.push_back(Regions::Player::Bow::bow1);
            regions.push_back(Regions::Player::Bow::bow2);
            regions.push_back(Regions::Player::Bow::bow3);
                    
            addTween(new AnimationTween2D(regions,0.1f,false));
            if(m_lookDirection == ODIN_DIRECTION_LEFT)
                m_offset = Vector2f(-18.0f,0.0f) * GAME_SCALE;
            setDrawSize(PLAYER_WIDTH/GAME_SCALE+18.0f,PLAYER_HEIGHT/GAME_SCALE);
            break;
        case PLAYER_BOW_FINISH:
            setTexture(m_sheet);
            setSrcRegion(Regions::Player::Bow::bow_finish);
            if(m_lookDirection == ODIN_DIRECTION_LEFT)
                m_offset = Vector2f(-18.0f,0.0f) * GAME_SCALE;
            setDrawSize((GLfloat)m_srcRegion.width,(GLfloat)m_srcRegion.height);
            break;
        case PLAYER_KNOCKBACK:
            setTexture(m_sheet);
            setSrcRegion(Regions::Player::Ground::Knockback::knockback);
            setDrawSize((GLfloat)m_srcRegion.width,(GLfloat)m_srcRegion.height);
            break;
        case PLAYER_KNOCKBACK_FALL:
            setTexture(m_sheet);
            setSrcRegion(Regions::Player::Fall::Knockback::knockback);
            setDrawSize((GLfloat)m_srcRegion.width,(GLfloat)m_srcRegion.height);
            break;
        default:
            break;
    }
    
    m_currentAnim = anim;
}

Johnny::Rectangle<GLfloat> Player::getHitbox() const
{
    Johnny::Rectangle<GLfloat> hitbox;
    hitbox.x = getPosition().x - m_offset.x;
    hitbox.y = getPosition().y - m_offset.y;
    hitbox.width = PLAYER_WIDTH;
    hitbox.height = PLAYER_HEIGHT;
    
    return hitbox;
}