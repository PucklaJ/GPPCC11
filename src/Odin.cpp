#include "../include/Odin.h"
#include "../include/Defines.h"
#include "../include/Player.h"
#include "../include/Speer.h"
#include "../include/Game.h"
#include "../include/BossfightScreen.h"
#include "../include/Lightning.h"
#include "../include/HealthBar.h"
#include "../include/Sounds.h"
#include <Johnny.h>

Odin* Odin::mainOdin = nullptr;

Odin::Odin() : Sprite2D("Odin.png")
{
    mainOdin = this;
    setName("Odin");
}

Odin::~Odin()
{
    
}

bool Odin::init()
{
    Sprite2D::init();
    
    setScale(GAME_SCALE,GAME_SCALE);
    
    Vector2f pos(m_mainClass->getNativeRes().x - ODIN_WIDTH,GROUND_POS_Y - ODIN_HEIGHT);
    
    setPosition(pos);
    
    m_velocity.x = -ODIN_VELOCITY;
    m_state = ODIN_WALK_STATE;
    m_walkDirection = ODIN_DIRECTION_LEFT;
    m_hitDirection = ODIN_DIRECTION_LEFT;
    
    setDepth(ODIN_DEPTH);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    loadAnimations();
    setAnimation(ODIN_STAND);
    
    m_healthBar = new HealthBar("heart2.png","heart3.png",Vector2f(m_mainClass->getNativeRes().width,0.0f),HEALTH_BAR_RIGHT,ODIN_GROUND_HEALTH);
    addChild(m_healthBar);
    
    return true;}

bool Odin::update()
{
    Sprite2D::update();
    updateKI();
    controlPhysics();
    
    return true;
}

bool Odin::render()
{
    setPosition(getPosition() + m_offset);
    
    Sprite2D::render();
    
    setPosition(getPosition() - m_offset);
    
    return true;
}

void Odin::quit()
{
    Sprite2D::quit();
}

void Odin::updateKI()
{
    m_time += m_mainClass->getTimer()->getDeltaTimeInSeconds();
    m_time1 += m_mainClass->getTimer()->getDeltaTimeInSeconds();
    
    if(Game::state == GAME_BOSS_GROUND_STATE || Game::state == GAME_BOSS_DESTROY_GROUND_STATE)
    {
        m_time1 = 0.0f;
        if(m_state == ODIN_WAIT_FOR_WALK_STATE)
        {
            setFlip(m_walkDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            setAnimation(ODIN_STAND);
            if(m_time >= ODIN_WAIT_FOR_WALK_TIME)
            {
                m_state = ODIN_WALK_STATE;
                m_velocity.x = m_walkDirection == ODIN_DIRECTION_LEFT ? ODIN_VELOCITY : -ODIN_VELOCITY;
                m_walkDirection = !m_walkDirection;
            }
        }
        else if(m_state == ODIN_WALK_STATE)
        {
            setAnimation(ODIN_RUN);
            setFlip(m_walkDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            
            m_time2 += m_mainClass->getTimer()->getDeltaTimeInSeconds();
            if(m_time2 >= 0.5f)
            {
                m_time2 = 0.0f;
                if(Sounds::walkWood[0]->isPlaying())
                    Sounds::walkWood[1]->play2d();
                else
                    Sounds::walkWood[0]->play2d();
            }
            
            if(m_time >= ODIN_WALK_TIME_TO_THROW)
            {
                setAnimation(ODIN_STAND);
                if(Game::phase >= 3)
                {
                    if(luck(60))
                    {
                        m_time = 0.0f;
                        m_state = ODIN_TELEPORT_WAIT_STATE;
                        Vector2f pos(Player::mainPlayer->getPosition() + (m_walkDirection == ODIN_DIRECTION_LEFT ? Vector2f(-ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH-ODIN_WIDTH,0.0f) : Vector2f(ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH,0.0f)));
                        pos.y = ODIN_TELEPORT_Y_POS;
                        
                        if(pos.x + ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH < 0.0f)
                        {
                            pos.x = ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH;
                        }
                        else if(pos.x + ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH - ODIN_WIDTH - ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH > m_mainClass->getNativeRes().width)
                        {
                            pos.x = m_mainClass->getNativeRes().x - ODIN_WIDTH - ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH;
                        }
                        
                        if(pos.x + ODIN_WIDTH > m_mainClass->getNativeRes().x)
                            pos.x = m_mainClass->getNativeRes().x - ODIN_WIDTH;
                        else if(pos.x < 0.0f)
                            pos.x = 0.0f;
                        setPosition(pos);
                        m_velocity.x = 0.0f;
                    }
                    else
                        throwSpeer();
                }
                else
                    throwSpeer();
                    
                
            }
            
            if(m_state == ODIN_WALK_STATE && Player::mainPlayer->getPosition().distance(getPosition() + getActualSize()/2.0f) < ODIN_HIT_ON_GROUND_DISTANCE)
            {
                setAnimation(ODIN_STAND);
                m_state = ODIN_WAIT_FOR_HIT_ON_GROUND_STATE;
                m_time = 0.0f;
                m_velocity.x = 0.0f;
                
                if((Player::mainPlayer->getPosition() - getPosition() - getActualSize()/2.0f).x > 0.0f)
                    m_hitDirection = ODIN_DIRECTION_RIGHT;
                else
                    m_hitDirection = ODIN_DIRECTION_LEFT;
                
                setFlip(m_hitDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
                
                setAnimation(Animation::ODIN_HIT_WAIT);
            }
        }
        else if(m_state == ODIN_WAIT_FOR_SECOND_SPEER_STATE)
        {
            setAnimation(Game::state == GAME_BOSS_GROUND_STATE ? Animation::ODIN_SPEER_WAIT : Animation::ODIN_SPEER_WAIT_FALL);
            if(m_time >= ODIN_WAIT_FOR_SECOND_SPEER_TIME)
            {
                m_time = 0.0f;
                throwSpeer();
            }
        }
        else if(m_state == ODIN_WAIT_FOR_GRAB_NEW_SPEER_STATE)
        {
            if(m_time >= ODIN_WAIT_FOR_GRAB_NEW_SPEER_TIME)
            {
                m_time = 0.0f;
                m_state = ODIN_WAIT_FOR_SECOND_SPEER_STATE;
            }
        }
        else if(m_state == ODIN_WAIT_FOR_WALK_SPEER_STATE)
        {
            if(m_time >= ODIN_WAIT_FOR_WALK_SPEER_TIME)
            {
                if(m_walkDirection == ODIN_DIRECTION_RIGHT)
                {
                    m_velocity.x = ODIN_VELOCITY;
                }
                else
                {
                    m_velocity.x = -ODIN_VELOCITY;
                }
                
                m_state = ODIN_WALK_STATE;
                m_time = 0.0f;
            }
        }
        else if(m_state == ODIN_WAIT_FOR_HIT_ON_GROUND_STATE)
        {
            setFlip(m_hitDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            if(m_time >= ODIN_WAIT_FOR_HIT_ON_GROUND_TIME)
            {
                hitOnGround();
                setAnimation(Animation::ODIN_HIT);
            }
        }
        else if(m_state == ODIN_HIT_ON_GROUND_STATE)
        {
            setFlip(m_hitDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            if(m_time >= ODIN_HIT_ON_GROUND_TIME)
            {
                m_state = ODIN_WAIT_FOR_WALK_HIT_ON_GROUND_STATE;
                m_time = 0.0f;
            }
            else
            {
                if(Player::mainPlayer->m_state != PLAYER_KNOCKBACK_STATE && hitHitBoxIntersects(Player::mainPlayer->getHitbox()))
                {
                    Player::mainPlayer->hitByHit(Vector2f(getHitHitbox().x,getHitHitbox().y) + Vector2f(getHitHitbox().width,getHitHitbox().height)/2.0f + Vector2f(getHitHitbox().width/4.0f,0.0f)*(m_hitDirection == ODIN_DIRECTION_LEFT ? 50.0f : -50.0f));
                    m_time = ODIN_HIT_ON_GROUND_TIME;
                }
            }
        }
        else if(m_state == ODIN_WAIT_FOR_WALK_HIT_ON_GROUND_STATE)
        {
            setFlip(m_hitDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            
            if(m_time >= ODIN_WAIT_FOR_WALK_HIT_ON_GROUND_TIME)
            {
                m_time = 0.0f;
                m_state = ODIN_WALK_STATE;
                if(m_walkDirection == ODIN_DIRECTION_RIGHT)
                    m_velocity.x = ODIN_VELOCITY;
                else
                    m_velocity.x = -ODIN_VELOCITY;
            }
        }
        else if(m_state == ODIN_KNOCKBACK_STATE)
        {
            if(m_time >= ODIN_KNOCKBACK_TIME)
            {
                m_state = ODIN_AWAKE_FROM_KNOCKBACK_STATE;
                m_time = 0.0f;
                stopTween(m_blinkID);
                setAnimation(Animation::ODIN_STAND);
            }
        }
        else if(m_state == ODIN_AWAKE_FROM_KNOCKBACK_STATE)
        {
            if(m_time >= ODIN_KNOCKBACK_EXPLOSION_TIME)
            {
                if(m_health <= 0.0f)
                {
                    setAnimation(Animation::ODIN_KNOCKBACK_EXPLOSION);
                    m_state = ODIN_DESTROY_GROUND_STATE_JUMP_UP;
                    Game::state = GAME_BOSS_DESTROY_GROUND_STATE;
                    m_blinkID = addTween(new PositionTween2D(m_mainClass->getNativeRes()/2.0f - getActualSize()/2.0f - Vector2f(0.0f,500.0f),ODIN_DESTROY_GROUND_JUMP_UP_TIME));
                    m_time = 0.0f;
                }
                else
                {
                    setAnimation(Animation::ODIN_KNOCKBACK_EXPLOSION);
                    m_state = ODIN_KNOCKBACK_EXPLOSION_STATE;
                    m_time = 0.0f;
                    if(Player::mainPlayer->getPosition().distance(getPosition()+getActualSize()/2.0f) <= ODIN_KNOCKBACK_EXPLOSION_DISTANCE)
                    {
                        Vector2f knock = (Player::mainPlayer->getPosition() - getPosition() - getActualSize()/2.0f);
                        knock.normalise();
                        Player::mainPlayer->m_velocity = knock * PLAYER_KNOCKBACK_POWER * ODIN_KNOCKBACK_EXPLOSION_AMPLIFIER; 
                    }
                    
                    Sounds::knockbackExplosion->play2d();
                }
            }
        }
        else if(m_state == ODIN_KNOCKBACK_EXPLOSION_STATE)
        {
            if(m_time >= ODIN_KNOCKBACK_EXPLOSION_TIME)
            {
                setAnimation(Animation::ODIN_STAND);
                m_state = ODIN_WALK_STATE;
                m_velocity.x = m_walkDirection == ODIN_DIRECTION_LEFT ? -ODIN_VELOCITY : ODIN_VELOCITY;
                m_time = 0.0f;
            }
        }
        else if(m_state == ODIN_DESTROY_GROUND_STATE_JUMP_UP)
        {
            if(!isRunning(m_blinkID))
            {
                setAnimation(Animation::ODIN_HIT);
                m_state = ODIN_DESTROY_GROUND_STATE_JUMP_DOWN;
                m_blinkID = addTween(new PositionTween2D(Vector2f(getPosition().x,GROUND_POS_Y - getActualSize().height),ODIN_DESTROY_GROUND_JUMP_DOWN_TIME));
            }
        }
        else if(m_state == ODIN_DESTROY_GROUND_STATE_JUMP_DOWN)
        {
            if(!isRunning(m_blinkID))
            {
                m_state = ODIN_DESTROY_GROUND_STATE;
                static_cast<BossfightScreen*>(getParent())->destroyGround();
                Game::state = GAME_GROUND_FALL_TRANSITION_STATE;
                
                Sounds::groundDestruction[getRand(0,3)]->play2d();
            }
        }
        else if(m_state == ODIN_TELEPORT_WAIT_STATE)
        {
            setAnimation(Animation::ODIN_HIT_WAIT);
            
            if(m_time >= ODIN_TELEPORT_WAIT_TIME)
            {
                m_hitDirection = ((Player::mainPlayer->getPosition() + Vector2f(PLAYER_WIDTH,PLAYER_HEIGHT))/2.0f - (getPosition() + Vector2f(ODIN_WIDTH,ODIN_HEIGHT)/2.0f)).x < 0.0f ? ODIN_DIRECTION_LEFT : ODIN_DIRECTION_RIGHT;
                
                setAnimation(Animation::ODIN_HIT);
                addTween(new PositionTween2D(Vector2f(getPosition().x,GROUND_POS_Y-ODIN_HEIGHT),ODIN_TELEPORT_MOVE_TIME));
                m_time = 0.0f;
                m_state = ODIN_TELEPORT_ATTACK_STATE;
                setFlip(m_hitDirection == ODIN_DIRECTION_LEFT ? Flip::NONE : Flip::HORIZONTALLY);
            }
        }
        else if(m_state == ODIN_TELEPORT_ATTACK_STATE)
        {
            if(Player::mainPlayer->getState() != PLAYER_KNOCKBACK_STATE &&
               hitHitBoxIntersects(Player::mainPlayer->getHitbox()))
            {
                Player::mainPlayer->hitByHit(Vector2f(getHitHitbox().x,getHitHitbox().y) + Vector2f(getHitHitbox().width,getHitHitbox().height)/2.0f);
            }
            
            if(getTweens().empty())
            {
                m_time = 0.0f;
                m_state = ODIN_WAIT_FOR_WALK_HIT_ON_GROUND_STATE;
            }
        }
        else if(m_state == ODIN_INTRO_STATE)
        {
            if(getTweens().empty())
            {
                m_state = ODIN_WALK_STATE;
                m_time = 0.0f;
            }
        }
    }
    else if(Game::state == GAME_GROUND_FALL_TRANSITION_STATE)
    {
        m_time = 0.0f;
        m_time1 = 0.0f;
        if(getPosition().y > m_mainClass->getNativeRes().y + GAME_GROUND_FALL_TRANSITION_OVERLAP)
        {
            transitionDone = true;
            m_state = ODIN_WALK_STATE;
            m_walkDirection = ODIN_DIRECTION_LEFT;
        }
    }
    else if(Game::state == GAME_BOSS_FALL_STATE)
    {
        setFlip(Flip::NONE);
        transitionDone = false;
        if(m_state == ODIN_WALK_STATE)
        {
            setAnimation(ODIN_FALL);
            if(getPosition().x != ODIN_FALL_WALK_X_POS && m_fallWalkID == 0)
            {
                stopTween(m_fallWalkID);
                m_fallWalkID = addTween(new PositionTween2D(Vector2f(ODIN_FALL_WALK_X_POS,0.0f),m_fallToWalkTime));
            }
            else if(getPosition().x == ODIN_FALL_WALK_X_POS)
            {
                if(m_time1 >= ODIN_THROW_LIGHTNING_TIME && (bool)ODIN_CAN_THROW_LIGHTNING)
                {
                    throwLightning();
                    m_previousLightningPos = m_previousLightningPos == ODIN_DIRECTION_UP ? ODIN_DIRECTION_DOWN : ODIN_DIRECTION_UP;
                }
                else if(m_time >= ODIN_WALK_TIME_TO_THROW)
                {
                    throwSpeer();
                }
                else
                {
                    if(m_walkDirection == ODIN_DIRECTION_LEFT)
                    {
                        m_velocity.y = ODIN_VELOCITY;
                    }
                    else
                    {
                        m_velocity.y = -ODIN_VELOCITY;
                    }
                }
            }
            else
            {
                m_time = 0.0f;
                m_fallToWalkTime -= m_mainClass->getTimer()->getDeltaTimeInSeconds();
            }
        }
        else if(m_state == ODIN_WAIT_FOR_WALK_STATE)
        {
            if(m_time >= ODIN_WAIT_FOR_WALK_TIME)
            {
                m_time = 0.0f;
                m_state = ODIN_WALK_STATE;
                m_walkDirection = !m_walkDirection;
            }
        }
        else if(m_state == ODIN_KNOCKBACK_STATE)
        {
            stopTween(m_fallWalkID);
            m_fallWalkID = 0;
            
            if(m_time >= ODIN_KNOCKBACK_TIME)
            {
                m_state = ODIN_AWAKE_FROM_KNOCKBACK_STATE;
                m_time = 0.0f;
                stopTween(m_blinkID);
                
                if(Game::phase != 3)
                    setAnimation(Animation::ODIN_FALL);
            }
        }
        else if(m_state == ODIN_AWAKE_FROM_KNOCKBACK_STATE)
        {
            if(m_time >= ODIN_KNOCKBACK_EXPLOSION_TIME)
            {
                m_time = 0.0f;
                
                if(m_health <= 0.0f)
                {
                    if(Game::phase == 3)
                        Game::state = GAME_FALL_WIN_TRANSITION_STATE;
                    else
                        Game::state = GAME_FALL_GROUND_TRANSITION_STATE;
                }
                else
                {
                    setAnimation(Animation::ODIN_KNOCKBACK_EXPLOSION);
                    m_state = ODIN_KNOCKBACK_EXPLOSION_STATE;
                    if(Player::mainPlayer->getPosition().distance(getPosition()+getActualSize()/2.0f) <= ODIN_KNOCKBACK_EXPLOSION_DISTANCE)
                    {
                        Vector2f knock = (Player::mainPlayer->getPosition() - getPosition() - getActualSize()/2.0f);
                        knock.normalise();
                        Player::mainPlayer->m_velocity = knock * PLAYER_KNOCKBACK_POWER * ODIN_KNOCKBACK_EXPLOSION_AMPLIFIER; 
                    }
                    
                    Sounds::knockbackExplosion->play2d();
                }
                
                m_time = 0.0f;
            }
        }
        else if(m_state == ODIN_KNOCKBACK_EXPLOSION_STATE)
        {
            if(m_time >= ODIN_KNOCKBACK_EXPLOSION_TIME)
            {
                setAnimation(Animation::ODIN_FALL);
                m_state = ODIN_WALK_STATE;
                m_time = 0.0f;
            }
        }
        else if(m_state == ODIN_WAIT_FOR_SECOND_SPEER_STATE)
        {
            setAnimation(Animation::ODIN_SPEER_WAIT_FALL);
            if(m_time >= ODIN_WAIT_FOR_SECOND_SPEER_TIME)
            {
                throwSpeer();
            }
        }
        else if(m_state == ODIN_WAIT_FOR_GRAB_NEW_SPEER_STATE)
        {
            if(m_time >= ODIN_WAIT_FOR_GRAB_NEW_SPEER_TIME)
            {
                m_state = ODIN_WAIT_FOR_SECOND_SPEER_STATE;
                m_time = 0.0f;
            }
        }
        else if(m_state == ODIN_WAIT_FOR_WALK_SPEER_STATE)
        {
            if(m_time >= ODIN_WAIT_FOR_WALK_SPEER_TIME)
            {
                m_time = 0.0f;
                m_state = ODIN_WALK_STATE;
            }
        }
        else if(m_state == ODIN_WAIT_FOR_LIGHTNING_THROW_STATE)
        {
            if(m_time >= ODIN_WAIT_FOR_LIGHTNING_THROW_TIME)
            {
                throwLightning();
            }
        }
    }
    else if(Game::state == GAME_FALL_GROUND_TRANSITION_STATE)
    {
        setAnimation(ODIN_STAND);
        if(getPosition().y >= ODIN_FALL_GROUND_TRANSITION_END_Y_POS)
        {
            setPosition(getPosition().x,ODIN_FALL_GROUND_TRANSITION_END_Y_POS);
            transitionDone = true;
        }
    }
    else if(Game::state == GAME_GAME_OVER_STATE)
    {
        if(m_state == ODIN_WAIT_FOR_WALK_STATE)
        {
            m_time1 = 0.0f;
            setFlip(m_walkDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            setAnimation(ODIN_STAND);
            if(m_time >= ODIN_WAIT_FOR_WALK_TIME)
            {
                m_state = ODIN_WALK_STATE;
                m_velocity.x = m_walkDirection == ODIN_DIRECTION_LEFT ? ODIN_VELOCITY : -ODIN_VELOCITY;
                m_walkDirection = !m_walkDirection;
            }
        }
        else if(m_state == ODIN_WALK_STATE)
        {
            m_time1 = 0.0f;
            m_time = 0.0f;
            setFlip(m_walkDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
            setAnimation(ODIN_RUN);
            setFlip(m_walkDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
        }
        else if(m_state == ODIN_FALL_GAME_OVER_STATE)
        {
            setAnimation(ODIN_FALL);
            setFlip(Flip::NONE);
            m_time = 0.0f;
            
            if(getPosition().x != ODIN_FALL_WALK_X_POS && m_fallWalkID == 0)
            {
                stopTween(m_fallWalkID);
                m_fallWalkID = addTween(new PositionTween2D(Vector2f(ODIN_FALL_WALK_X_POS,0.0f),m_fallToWalkTime));
            }
            else if(getPosition().x == ODIN_FALL_WALK_X_POS)
            {
                if(m_walkDirection == ODIN_DIRECTION_LEFT)
                {
                    m_velocity.y = ODIN_VELOCITY;
                }
                else
                {
                    m_velocity.y = -ODIN_VELOCITY;
                }
            }
            else
            {
                m_time = 0.0f;
                m_fallToWalkTime -= m_mainClass->getTimer()->getDeltaTimeInSeconds();
            }
        }
        else if(m_state == ODIN_WAIT_FOR_WALK_GAME_OVER_STATE)
        {
            setAnimation(ODIN_FALL);
            setFlip(Flip::NONE);
            
            if(m_time >= ODIN_WAIT_FOR_WALK_TIME)
            {
                m_time = 0.0f;
                m_state = ODIN_FALL_GAME_OVER_STATE;
                m_walkDirection = !m_walkDirection;
            }
        }
    }
    
}

void Odin::controlPhysics()
{
    if(m_state == ODIN_INTRO_STATE)
        return;
    
    if(Game::state == GAME_BOSS_GROUND_STATE)
    {
        m_velocity += Player::gravity * m_mainClass->getTimer()->getDeltaTimeInSeconds();
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());

        if(getPosition().y + getActualSize().height > GROUND_POS_Y)
        {
            setPosition(Vector2f(getPosition().x,GROUND_POS_Y) - Vector2f(0.0f,getActualSize().height));
            m_velocity.y = 0.0f;
        }

        if(getPosition().x + ODIN_WIDTH > m_mainClass->getNativeRes().x)
        {
            setPosition(m_mainClass->getNativeRes().x-getActualSize().width,getPosition().y);
            m_state = ODIN_WAIT_FOR_WALK_STATE;
            m_velocity.x = 0.0f;
            m_time = 0.0f;
        }
        else if(getPosition().x < 0.0f)
        {
            setPosition(0.0f,getPosition().y);
            m_state = ODIN_WAIT_FOR_WALK_STATE;
            m_velocity.x = 0.0f;
            m_time = 0.0f;
        }
    }
    else if(Game::state == GAME_GROUND_FALL_TRANSITION_STATE)
    {
        m_velocity = Player::gravity;
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
    }
    else if(Game::state == GAME_BOSS_FALL_STATE)
    {
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        
        if(getPosition().y + getActualSize().height > m_mainClass->getNativeRes().height)
        {
            setPosition(getPosition().x,m_mainClass->getNativeRes().height-getActualSize().height);
            m_state = ODIN_WAIT_FOR_WALK_STATE;
            m_velocity.y = 0.0f;
        }
        else if(getPosition().y < 0.0f)
        {
            setPosition(getPosition().x,0.0f);
            m_state = ODIN_WAIT_FOR_WALK_STATE;
            m_velocity.y = 0.0f;
        }
    }
    else if(Game::state == GAME_FALL_INTRO_STATE)
    {
        setAnimation(Animation::ODIN_FALL);
        
        m_time = 0.0f;
        if(getPosition().y < ODIN_FALL_Y_END_POS)
        {
            m_velocity = Player::gravity;
            addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        }
        else
        {
            setPosition(getPosition().x,ODIN_FALL_Y_END_POS);
            introDone = true;
            m_velocity = Vector2f(0.0f,0.0f);
        }
        
        m_health = ODIN_FALL_HEALTH;
        m_healthBar->setMaxHealth(ODIN_FALL_HEALTH);
        if(m_healthBar->getHealth() <= 0.0f)
            m_healthBar->reduce(-ODIN_FALL_HEALTH);
        m_fallToWalkTime = 1.0f;
    }
    else if(Game::state == GAME_FALL_GROUND_TRANSITION_STATE)
    {
        m_time = 0.0f;
        m_velocity = Player::gravity;
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
    }
    else if(Game::state == GAME_GROUND_INTRO_STATE)
    {
        m_time = 0.0f;
        m_velocity = Player::gravity;
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        
        if(getPosition().y + ODIN_HEIGHT >= GROUND_POS_Y)
        {
            setPosition(getPosition().x,GROUND_POS_Y-ODIN_HEIGHT);
            m_velocity.x = m_walkDirection == ODIN_DIRECTION_LEFT ? -ODIN_VELOCITY : ODIN_VELOCITY;
            introDone = true;
        }
        
        m_state = ODIN_WALK_STATE;
        m_health = ODIN_GROUND_HEALTH;
        m_healthBar->setMaxHealth(ODIN_GROUND_HEALTH);
        if(m_healthBar->getHealth() <= 0.0f)
            m_healthBar->reduce(-ODIN_GROUND_HEALTH);
    }
    else if(Game::state == GAME_GAME_OVER_STATE)
    {
        if(m_state == ODIN_WALK_STATE)
        {
            m_velocity += Player::gravity * m_mainClass->getTimer()->getDeltaTimeInSeconds();
            addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());

            if(getPosition().y + getActualSize().height > GROUND_POS_Y)
            {
                setPosition(Vector2f(getPosition().x,GROUND_POS_Y) - Vector2f(0.0f,getActualSize().height));
                m_velocity.y = 0.0f;
            }

            if(getPosition().x + getActualSize().width > m_mainClass->getNativeRes().x)
            {
                setPosition(m_mainClass->getNativeRes().x-getActualSize().width,getPosition().y);
                m_state = ODIN_WAIT_FOR_WALK_STATE;
                m_velocity.x = 0.0f;
            }
            else if(getPosition().x < 0.0f)
            {
                setPosition(0.0f,getPosition().y);
                m_state = ODIN_WAIT_FOR_WALK_STATE;
                m_velocity.x = 0.0f;
            }
        }
        else if(m_state == ODIN_FALL_GAME_OVER_STATE)
        {
            addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        
            if(getPosition().y + getActualSize().height > m_mainClass->getNativeRes().height)
            {
                setPosition(getPosition().x,m_mainClass->getNativeRes().height-getActualSize().height);
                m_state = ODIN_WAIT_FOR_WALK_GAME_OVER_STATE;
                m_velocity.y = 0.0f;
            }
            else if(getPosition().y < 0.0f)
            {
                setPosition(getPosition().x,0.0f);
                m_state = ODIN_WAIT_FOR_WALK_GAME_OVER_STATE;
                m_velocity.y = 0.0f;
            }
        }
    }
    else if(Game::state == GAME_FALL_WIN_TRANSITION_STATE)
    {
        m_velocity = Player::gravity;
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        
        if(getPosition().y > ODIN_FALL_GROUND_TRANSITION_END_Y_POS)
            transitionDone = true;
    }
    else if(Game::state == GAME_GROUND_WIN_INTRO_STATE)
    {
        m_time = 0.0f;
        m_velocity = Player::gravity;
        addPosition(m_velocity * m_mainClass->getTimer()->getDeltaTimeInSeconds());
        
        if(getPosition().y + ODIN_HEIGHT >= GROUND_POS_Y)
        {
            setPosition(getPosition().x,GROUND_POS_Y-ODIN_HEIGHT);
            m_velocity.x = m_walkDirection == ODIN_DIRECTION_LEFT ? -ODIN_VELOCITY : ODIN_VELOCITY;
            introDone = true;
        }
    }
    
}

void Odin::throwSpeer()
{
    m_time = 0.0f;
    m_velocity = Vector2f(0.0f,0.0f);
    
    Vector2f speerdir = (Player::mainPlayer->getPosition() + Player::mainPlayer->getActualSize()/2.0f) - (getPosition() + getActualSize()/2.0f);
    speerdir.normalise();
    
    if(speerdir.x > 0.0f)
        m_hitDirection = ODIN_DIRECTION_RIGHT;
    else
        m_hitDirection = ODIN_DIRECTION_LEFT;
    
    setFlip(m_hitDirection == ODIN_DIRECTION_RIGHT ? Flip::HORIZONTALLY : Flip::NONE);
    
    if(m_state == ODIN_WAIT_FOR_SECOND_SPEER_STATE)
    {
        setAnimation(Game::state == GAME_BOSS_GROUND_STATE ? Animation::ODIN_SPEER_THROW : Animation::ODIN_SPEER_THROW_FALL);
        getParent()->addChild(new Speer((getPosition() + getActualSize()/2.0f),speerdir * ODIN_SPEER_THROW_VELOCITY));
        if(++m_thrownSpeers >= ODIN_NUM_THROW_SPEERS)
        {
            m_thrownSpeers = 0;
            m_state = ODIN_WAIT_FOR_WALK_SPEER_STATE;
        }
        else
        {
            m_state = ODIN_WAIT_FOR_GRAB_NEW_SPEER_STATE;
        }
        
        Sounds::speer->play2d();
    }
    else
        m_state = ODIN_WAIT_FOR_SECOND_SPEER_STATE;
}

void Odin::throwLightning()
{
    m_time = 0.0f;
    m_time1 = 0.0f;
    m_velocity = Vector2f(0.0f,0.0f);
    if(m_state == ODIN_WAIT_FOR_LIGHTNING_THROW_STATE)
    {
        getParent()->addChild(new Lightning(getPosition() + getActualSize()/2.0f));
        m_state = ODIN_WAIT_FOR_WALK_SPEER_STATE;
        setAnimation(ODIN_LIGHTNING_THROW);
    }
    else
    {
        Sounds::lightning_throw[getRand(0,1)]->play2d(true);
        m_state = ODIN_WAIT_FOR_LIGHTNING_THROW_STATE;
        setAnimation(Animation::ODIN_LIGHTNING_WAIT);
    }
}

void Odin::hitOnGround()
{
    m_time = 0.0f;
    m_velocity.x = 0.0f;
    m_state = ODIN_HIT_ON_GROUND_STATE;
    
    Sounds::axe[getRand(0,1)]->play2d();
    
    //m_hitbox->setPosition(getPosition() + (m_hitDirection == ODIN_DIRECTION_LEFT ? Vector2f(ODIN_HIT_ON_GROUND_HIT_BOX_X_POS - ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH,ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS - ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT) : Vector2f(ODIN_WIDTH + ODIN_HIT_ON_GROUND_HIT_BOX_X_POS,ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS - ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT)));
    //m_hitbox->setVisible(true);
}

void Odin::damage(float dmg)
{
    setAnimation(Animation::ODIN_KNOCKBACK);
    
    if(m_state == ODIN_WAIT_FOR_LIGHTNING_THROW_STATE)
    {
        Sounds::lightning_throw[0]->stop();
        Sounds::lightning_throw[1]->stop();
    }
    
    m_health -= dmg;
    m_state = ODIN_KNOCKBACK_STATE;
    m_time = 0.0f;
    m_velocity = Vector2f(0.0f,0.0f);
    m_blinkID = addTween(new BlinkTween2D(0.1f));
    m_healthBar->reduce(dmg);
    
    Sounds::odinPain->play2d();
}

void Odin::setFlip(Flip flip)
{
    m_stand->setFlip(flip);
    m_run1->setFlip(flip);
    m_run2->setFlip(flip);
    m_run25->setFlip(flip);
    m_run15->setFlip(flip);
    m_speer->setFlip(flip);
    m_speer1->setFlip(flip);
    m_speer2->setFlip(flip);
    m_hit->setFlip(flip);
    m_hit1->setFlip(flip);
    m_hit2->setFlip(flip);
    m_hit3->setFlip(flip);
    m_knockback->setFlip(flip);
    m_knockback_explosion->setFlip(flip);
}

void Odin::loadAnimations()
{
    m_stand = m_mainClass->getResourceManager()->loadTexture("Odin.png",Settings::geti(SPRITE2D_FILTERING));
    m_run1 = m_mainClass->getResourceManager()->loadTexture("Odin1.png",Settings::geti(SPRITE2D_FILTERING));
    m_run15 = m_mainClass->getResourceManager()->loadTexture("Odin2.png",Settings::geti(SPRITE2D_FILTERING));
    m_run2 = m_mainClass->getResourceManager()->loadTexture("Odin3.png",Settings::geti(SPRITE2D_FILTERING));
    m_run25 = m_mainClass->getResourceManager()->loadTexture("Odin4.png",Settings::geti(SPRITE2D_FILTERING));
    m_fall = m_mainClass->getResourceManager()->loadTexture("Odin5.png",Settings::geti(SPRITE2D_FILTERING));
    m_fall15 = m_mainClass->getResourceManager()->loadTexture("Odin6.png",Settings::geti(SPRITE2D_FILTERING));
    m_fall1 = m_mainClass->getResourceManager()->loadTexture("Odin7.png",Settings::geti(SPRITE2D_FILTERING));
    m_fall25 = m_mainClass->getResourceManager()->loadTexture("Odin8.png",Settings::geti(SPRITE2D_FILTERING));
    m_fall2 = m_mainClass->getResourceManager()->loadTexture("Odin9.png",Settings::geti(SPRITE2D_FILTERING));
    m_speer = m_mainClass->getResourceManager()->loadTexture("Odin10.png",Settings::geti(SPRITE2D_FILTERING));
    m_speer1 = m_mainClass->getResourceManager()->loadTexture("Odin11.png",Settings::geti(SPRITE2D_FILTERING));
    m_speer2 = m_mainClass->getResourceManager()->loadTexture("Odin12.png",Settings::geti(SPRITE2D_FILTERING));
    m_speer_fall = m_mainClass->getResourceManager()->loadTexture("Odin13.png",Settings::geti(SPRITE2D_FILTERING));
    m_speer_fall1 = m_mainClass->getResourceManager()->loadTexture("Odin14.png",Settings::geti(SPRITE2D_FILTERING));
    m_speer_fall2 = m_mainClass->getResourceManager()->loadTexture("Odin15.png",Settings::geti(SPRITE2D_FILTERING));
    m_hit = m_mainClass->getResourceManager()->loadTexture("Odin16.png",Settings::geti(SPRITE2D_FILTERING));
    m_hit1 = m_mainClass->getResourceManager()->loadTexture("Odin17.png",Settings::geti(SPRITE2D_FILTERING));
    m_hit2 = m_mainClass->getResourceManager()->loadTexture("Odin18.png",Settings::geti(SPRITE2D_FILTERING));
    m_hit3 = m_mainClass->getResourceManager()->loadTexture("Odin19.png",Settings::geti(SPRITE2D_FILTERING));
    m_lightning1 = m_mainClass->getResourceManager()->loadTexture("Odin20.png",Settings::geti(SPRITE2D_FILTERING));
    m_lightning2 = m_mainClass->getResourceManager()->loadTexture("Odin21.png",Settings::geti(SPRITE2D_FILTERING));
    m_knockback = m_mainClass->getResourceManager()->loadTexture("Odin23.png",Settings::geti(SPRITE2D_FILTERING));
    m_knockback_explosion = m_mainClass->getResourceManager()->loadTexture("Odin24.png",Settings::geti(SPRITE2D_FILTERING));
}

void Odin::setAnimation(Animation anim)
{
    if(m_currentAnim == anim)
        return;
    
    clearTweens();
    
    m_offset.x = m_offset.y = 0.0f;
    
    std::vector<Texture*> animation;
    
    switch(anim)
    {
    case ODIN_STAND:
        setTexture(m_stand);
        break;
    case ODIN_RUN:
        animation.push_back(m_run15);
        animation.push_back(m_run1);
        animation.push_back(m_stand);
        animation.push_back(m_run25);
        animation.push_back(m_run2);
        animation.push_back(m_stand);
        
        addTween(new AnimationTween2D(animation,0.27f,true));
        break;
    case ODIN_FALL:
        animation.push_back(m_fall);
        animation.push_back(m_fall15);
        animation.push_back(m_fall1);
        animation.push_back(m_fall15);
        animation.push_back(m_fall);
        animation.push_back(m_fall25);
        animation.push_back(m_fall2);
        animation.push_back(m_fall25);
        
        addTween(new AnimationTween2D(animation,0.1f,true));
        break;
    case ODIN_SPEER_WAIT:
        setTexture(m_speer);
        if(m_hitDirection == ODIN_DIRECTION_RIGHT)
            m_offset = Vector2f(-20.0f,0.0f) * GAME_SCALE;
        break;
    case ODIN_SPEER_THROW:
        animation.push_back(m_speer1);
        animation.push_back(m_speer);
        animation.push_back(m_speer2);
        
        addTween(new AnimationTween2D(animation,0.05f,false));
        if(m_hitDirection == ODIN_DIRECTION_RIGHT)
            m_offset = Vector2f(-20.0f,0.0f) * GAME_SCALE;
        break;
    case ODIN_SPEER_WAIT_FALL:
        setTexture(m_speer_fall);
        break;
    case ODIN_SPEER_THROW_FALL:
        animation.push_back(m_speer_fall1);
        animation.push_back(m_speer_fall);
        animation.push_back(m_speer_fall2);
        
        addTween(new AnimationTween2D(animation,0.1f,false));
        break;
    case ODIN_HIT_WAIT:
        //setTexture(m_hit);
        animation.push_back(m_hit2);
        animation.push_back(m_hit1);
        animation.push_back(m_hit);
        
        addTween(new AnimationTween2D(animation,ODIN_WAIT_FOR_HIT_ON_GROUND_TIME/4.0f,false));
        
        if(m_hitDirection == ODIN_DIRECTION_LEFT)
            m_offset = Vector2f(-50.0f,0.0f) * GAME_SCALE;
        break;
    case ODIN_HIT:
        animation.push_back(m_hit1);
        animation.push_back(m_hit2);
        animation.push_back(m_hit3);
        
        addTween(new AnimationTween2D(animation,0.05f,false));
        if(m_hitDirection == ODIN_DIRECTION_LEFT)
            m_offset = Vector2f(-50.0f,0.0f) * GAME_SCALE;
        break;
    case ODIN_LIGHTNING_WAIT:
        setTexture(m_lightning1);
        break;
    case ODIN_LIGHTNING_THROW:
        animation.push_back(m_lightning2);
        animation.push_back(m_lightning1);
        animation.push_back(m_speer2);
        
        addTween(new AnimationTween2D(animation,0.1f,false));
        break;
    case ODIN_KNOCKBACK:
        setTexture(m_knockback);
        break;
    case ODIN_KNOCKBACK_EXPLOSION:
        setTexture(m_knockback_explosion);
        break;
    default:
        break;
    }
    
    m_currentAnim = anim;
}

bool Odin::hitBoxIntersects(Sprite2D* spr)
{
    Johnny::Rectangle<GLfloat> hitbox;
    
    hitbox.x = getPosition().x;
    hitbox.y = getPosition().y;
    hitbox.width = ODIN_WIDTH;
    hitbox.height = ODIN_HEIGHT;
    
    return spr->intersects(hitbox);
}
bool Odin::hitBoxIntersects(const Johnny::Rectangle<GLfloat>& rect)
{
    Johnny::Rectangle<GLfloat> hitbox;
    
    hitbox.x = getPosition().x;
    hitbox.y = getPosition().y;
    hitbox.width = ODIN_WIDTH;
    hitbox.height = ODIN_HEIGHT;
    
    return rect.intersects(hitbox);
}

bool Odin::hitHitBoxIntersects(const Johnny::Rectangle<GLfloat>& rect)
{
    Johnny::Rectangle<GLfloat> hitbox;
    
    Vector2f pos = getPosition() + 
                         (m_hitDirection == ODIN_DIRECTION_LEFT ?
                                        Vector2f(-ODIN_HIT_ON_GROUND_HIT_BOX_X_POS -
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH,
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS -
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT) :
                                        Vector2f(ODIN_WIDTH + ODIN_HIT_ON_GROUND_HIT_BOX_X_POS,
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS -
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT));
    
    hitbox.x = pos.x;
    hitbox.y = pos.y;
    hitbox.width = ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH;
    hitbox.height = ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT;
    
    return rect.intersects(hitbox);
}

Johnny::Rectangle<GLfloat> Odin::getHitHitbox()
{
    Johnny::Rectangle<GLfloat> hitbox;
    
    /*m_hitbox->setPosition(getPosition() + 
                         (m_hitDirection == ODIN_DIRECTION_LEFT ?
                                        Vector2f(ODIN_HIT_ON_GROUND_HIT_BOX_X_POS -
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH,
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS -
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT) :
                                        Vector2f(ODIN_WIDTH + ODIN_HIT_ON_GROUND_HIT_BOX_X_POS,
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS -
                                                 ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT)));*/
    
    hitbox.x = getPosition().x + m_hitDirection == ODIN_DIRECTION_LEFT ? (ODIN_HIT_ON_GROUND_HIT_BOX_X_POS - ODIN_HIT_ON_GROUND_HIT_BOX_WIDTH) : (ODIN_WIDTH + ODIN_HIT_ON_GROUND_HIT_BOX_X_POS);
    hitbox.y = getPosition().y + (ODIN_HIT_ON_GROUND_HIT_BOX_Y_POS - ODIN_HIT_ON_GROUND_HIT_BOX_HEIGHT);
    hitbox.width = ODIN_WIDTH;
    hitbox.height = ODIN_HEIGHT;
    
    return hitbox;
}