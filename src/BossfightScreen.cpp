#include "../include/BossfightScreen.h"
#include "../include/Game.h"
#include "../include/GameOverScreen.h"
#include "../include/Background.h"
#include "../include/WinScreen.h"
#include <Johnny.h>

BossfightScreen* BossfightScreen::mainBossfightScreen = nullptr;

BossfightScreen::BossfightScreen()
{
    m_visible = false;
    mainBossfightScreen = this;
}

BossfightScreen::~BossfightScreen()
{
    
}

bool BossfightScreen::init()
{
    //addBoundaries();
    
    //m_player = new Player();
    //addChild(m_player);
    
    //m_odin = new Odin();
    //addChild(m_odin);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    m_currentArrowPickupSpawnTime = ((float)getRand((int)(RAND_PRECISION*ARROW_PICKUP_MIN_SPAWN_TIME),(int)(RAND_PRECISION*ARROW_PICKUP_MAX_SPAWN_TIME)))/RAND_PRECISION;
    
    m_background = new Background();
    addChild(m_background);
    
    Game::phase = 1;
    
    return true;
}

bool BossfightScreen::update()
{
    m_time += m_mainClass->getTimer()->getDeltaTimeInSeconds();
    m_time1 += m_mainClass->getTimer()->getDeltaTimeInSeconds();
    
    if(!m_groundDestroyParts.empty())
    {
        bool allDone = true;
    
        for(size_t i = 0;i<m_groundDestroyParts.size();i++)
        {
            if(m_groundDestroyParts[i]->getTweens().size() == 2)
            {
                allDone = false;
                break;
            }
        }
        
        if(allDone)
        {
            for(size_t i = 0;i<m_groundDestroyParts.size();i++)
            {
                removeChild(m_groundDestroyParts[i],true,true);
            }
            
            m_groundDestroyParts.clear();
        }
    }
    
    handleStates();
    if(Game::state != GAME_MENU_STATE)
        handleArrowPickups();
    
    if(Game::state != GAME_MENU_STATE && Game::state != GAME_GAME_OVER_STATE)
    {
        if(Player::mainPlayer->getHealth() <= 0.0f || m_mainClass->getInputManager()->justPressed(Keys::U))
        {
            Player::mainPlayer->detach();
            
            getParent()->addChild(new GameOverScreen(),false);
            removeChild(m_player);
            m_player = nullptr;
            Player::mainPlayer = nullptr;
            
            if(Game::state == GAME_BOSS_GROUND_STATE)
            {
                Odin::mainOdin->m_state = ODIN_WALK_STATE;
                Odin::mainOdin->m_velocity.x = Odin::mainOdin->m_walkDirection == ODIN_DIRECTION_LEFT ? -ODIN_VELOCITY : ODIN_VELOCITY;
            }
            else if(Game::state == GAME_BOSS_FALL_STATE)
            {
                Odin::mainOdin->m_state = ODIN_FALL_GAME_OVER_STATE;
            }
            
            Game::state = GAME_GAME_OVER_STATE;
            
        }
    }
    
    
    
    return true;
}

void BossfightScreen::quit()
{
    m_arrowPickups.clear();
    m_obstacles.clear();
    m_arrowPickupTimes.clear();
}

void BossfightScreen::handleArrowPickups()
{
    if(m_time1 >= m_currentArrowPickupSpawnTime)
    {
        m_time1 = 0.0f;
        m_currentArrowPickupSpawnTime = ((float)getRand((int)(RAND_PRECISION*ARROW_PICKUP_MIN_SPAWN_TIME),(int)(RAND_PRECISION*ARROW_PICKUP_MAX_SPAWN_TIME)))/RAND_PRECISION;
        if(m_arrowPickups.size() < ARROW_PICKUP_MAX)
            spawnArrowPickup();
    }
    
    if(Game::state == GAME_BOSS_GROUND_STATE)
    {
        for(size_t i = 0;i<m_arrowPickups.size();i++)
        {
            if(m_arrowPickups[i]->getTweens().empty())
            {
                if(m_arrowPickupTimes[i] <= 0.0f)
                {
                    m_arrowPickups[i]->addTween(new BlinkTween2D(0.15f));
                    m_arrowPickupTimes[i] = ARROW_PICKUP_BLINK_TIME;
                }
                else
                {
                    m_arrowPickupTimes[i] -= m_mainClass->getTimer()->getDeltaTimeInSeconds();
                }
            }
            else if(m_arrowPickupTimes[i] <= 0.0f)
            {
                removeChild(m_arrowPickups[i],true,true);
                m_arrowPickups[i] = m_arrowPickups.back();
                m_arrowPickupTimes[i] = m_arrowPickupTimes.back();
                m_arrowPickups.pop_back();
                m_arrowPickupTimes.pop_back();
                i--;
                continue;
            }
            else if(m_arrowPickups[i]->getTweens()[0]->getTime() == 0.15f)
            {
                m_arrowPickupTimes[i] -= m_mainClass->getTimer()->getDeltaTimeInSeconds();
            }
        }
    }
    else if(Game::state == GAME_BOSS_FALL_STATE)
    {
        for(size_t i = 0;i<m_arrowPickups.size();i++)
        {
            if(m_arrowPickups[i]->getTweens().empty())
            {
                removeChild(m_arrowPickups[i],true,true);
                m_arrowPickups[i] = m_arrowPickups.back();
                m_arrowPickupTimes[i] = m_arrowPickupTimes.back();
                m_arrowPickups.pop_back();
                m_arrowPickupTimes.pop_back();
                i--;
                continue;
            }
        }
    }
    else if(Game::state == GAME_GROUND_FALL_TRANSITION_STATE)
    {
        for(size_t i = 0;i<m_arrowPickups.size();i++)
        {
            if(m_arrowPickups[i]->getPosition().y >= m_mainClass->getNativeRes().height)
            {
                removeChild(m_arrowPickups[i],true,true);
                m_arrowPickups[i] = m_arrowPickups.back();
                m_arrowPickupTimes[i] = m_arrowPickupTimes.back();
                m_arrowPickups.pop_back();
                m_arrowPickupTimes.pop_back();
                i--;
                continue;
            }
            else if(m_arrowPickups[i]->getTweens().empty() || m_arrowPickups[i]->getTweens()[0]->getTime() == 0.15f)
            {
                m_arrowPickups[i]->addPosition(0.0f,Player::gravity.y * m_mainClass->getTimer()->getDeltaTimeInSeconds());
            }
        }
    }
    else if(Game::state == GAME_FALL_INTRO_STATE)
    {
        for(size_t i = 0;i<m_arrowPickups.size();i++)
        {
            removeChild(m_arrowPickups[i],true,true);
        }
        
        m_arrowPickups.clear();
        m_arrowPickupTimes.clear();
    }
    
    
    if(Player::mainPlayer)
    {
        for(size_t i = 0;i<m_arrowPickups.size();i++)
        {
            if(m_arrowPickups[i]->intersects(Player::mainPlayer->getHitbox()))
            {
                Player::mainPlayer->collectArrowPickup();
                removeChild(m_arrowPickups[i],true,true);
                m_arrowPickups[i] = m_arrowPickups.back();
                m_arrowPickupTimes[i] = m_arrowPickupTimes.back();
                m_arrowPickups.pop_back();
                m_arrowPickupTimes.pop_back();
                i--;
                continue;
            }
        }
    }
    
    
}

void BossfightScreen::handleObstacles()
{
    if(m_background->canSpawnObstacles() && 
      (((Player::mainPlayer &&
         Player::mainPlayer->getState() != PLAYER_INTRO_STATE) ||
        !Player::mainPlayer) &&
        m_obstacles.size() < (size_t)MAX_OBSTACLES))
    {
        if(m_time >= m_currentObstacleSpawnTime)
        {
            m_time = 0.0f;
            spawnObstacle();
            m_currentObstacleSpawnTime = ((float)getRand((int)(RAND_PRECISION*OBSTACLE_MIN_SPAWN_TIME),(int)(RAND_PRECISION*OBSTACLE_MAX_SPAWN_TIME)))/RAND_PRECISION;
        }
    }
    else if(m_background->canSpawnObstacles())
    {
        m_time = 0.0f;
    }
    
    for(size_t i = 0;i<m_obstacles.size();i++)
    {
        if(Player::mainPlayer &&
           Player::mainPlayer->getState() != PLAYER_KNOCKBACK_STATE &&
           Player::mainPlayer->getState() != PLAYER_INTRO_STATE &&
           m_obstacles[i]->intersects(Player::mainPlayer->getHitbox()))
        {
            Vector2f vel = Player::mainPlayer->getPosition() - m_obstacles[i]->getPosition();
            vel.normalise();
            Player::mainPlayer->hitByObstacle(vel);
        }
        else if(m_obstacles[i]->getTweens().empty())
        {
            removeChild(m_obstacles[i],true,true);
            m_obstacles[i] = m_obstacles.back();
            m_obstacles.pop_back();
        }
    }
}

void BossfightScreen::handleStates()
{
    if(Game::state != GAME_BOSS_FALL_STATE &&
       Game::state != GAME_MENU_STATE &&
       Game::state != GAME_INTRO_STATE &&
       Game::state != GAME_GAME_OVER_STATE)
    {
        m_time = 0.0f;
    }
    
    if(Game::state != GAME_BOSS_FALL_STATE &&
       Game::state != GAME_BOSS_GROUND_STATE)
    {
        m_time1 = 0.0f;
    }
    
    if(Game::state == GAME_GROUND_FALL_TRANSITION_STATE)
    {
        if(Player::mainPlayer->transitionDone && Odin::mainOdin->transitionDone)
        {
            Player::mainPlayer->setPosition(Player::mainPlayer->getPosition().x,PLAYER_FALL_INTRO_Y_POS);
            Odin::mainOdin->setPosition(Odin::mainOdin->getPosition().x,ODIN_FALL_INTRO_Y_POS);
            
            Player::mainPlayer->transitionDone = false;
            Odin::mainOdin->transitionDone = false;
            
            Game::state = GAME_FALL_INTRO_STATE;
        }
    }
    else if(Game::state == GAME_FALL_INTRO_STATE)
    {
        if(Player::mainPlayer->introDone && Odin::mainOdin->introDone)
        {
            Game::state = GAME_BOSS_FALL_STATE;
            
            Player::mainPlayer->introDone = false;
            Odin::mainOdin->introDone = false;
        }
        
        if(m_currentObstacleSpawnTime == 0.0f)
        {
            m_currentObstacleSpawnTime = ((float)getRand((int)(RAND_PRECISION*OBSTACLE_MIN_SPAWN_TIME),(int)(RAND_PRECISION*OBSTACLE_MAX_SPAWN_TIME)))/RAND_PRECISION;
        }
    }
    else if(Game::state == GAME_BOSS_FALL_STATE || Game::state == GAME_MENU_STATE ||
            Game::state == GAME_INTRO_STATE || (Game::state == GAME_GAME_OVER_STATE && !m_ground))
    {
        handleObstacles();
    }
    else if(Game::state == GAME_FALL_GROUND_TRANSITION_STATE)
    {
        if(Player::mainPlayer->transitionDone && Odin::mainOdin->transitionDone)
        {
            Game::state = GAME_GROUND_INTRO_STATE;
            Player::mainPlayer->setPosition(Player::mainPlayer->getPosition().x,PLAYER_GROUND_INTRO_START_Y_POS);
            Odin::mainOdin->setPosition(Odin::mainOdin->getPosition().x,ODIN_GROUND_INTRO_START_Y_POS);
            Player::mainPlayer->transitionDone = false;
            Odin::mainOdin->transitionDone = false;
            if(++Game::phase > 3)
                Game::phase--;
            
            if(!m_ground)
            {
                addBoundaries();
                m_ground->setPosition(GROUND_POS_X,m_mainClass->getNativeRes().height);
            }
        }
    }
    else if(Game::state == GAME_GROUND_INTRO_STATE)
    {
        if(Player::mainPlayer->introDone && Odin::mainOdin->introDone)
        {
            Game::state = GAME_BOSS_GROUND_STATE;
            Player::mainPlayer->introDone = false;
            Odin::mainOdin->introDone = false;
        }
        
        if(m_ground->getTweens().empty())
        {
            m_ground->addTween(new PositionTween2D(Vector2f(GROUND_POS_X,_GROUND_POS_Y_(true)),1.0f));
        }
    }
    else if(Game::state == GAME_FALL_WIN_TRANSITION_STATE)
    {
        if(Player::mainPlayer->transitionDone && Odin::mainOdin->transitionDone)
        {
            Game::state = GAME_GROUND_WIN_INTRO_STATE;
            Player::mainPlayer->setPosition(Player::mainPlayer->getPosition().x,PLAYER_GROUND_INTRO_START_Y_POS);
            Odin::mainOdin->setPosition(Odin::mainOdin->getPosition().x,ODIN_GROUND_INTRO_START_Y_POS);
            Player::mainPlayer->transitionDone = false;
            Odin::mainOdin->transitionDone = false;
            
            if(!m_ground)
            {
                addBoundaries();
                m_ground->setPosition(GROUND_POS_X,m_mainClass->getNativeRes().height);
            }
        }
    }
    else if(Game::state == GAME_GROUND_WIN_INTRO_STATE)
    {
        if(Player::mainPlayer->introDone && Odin::mainOdin->introDone)
        {
            Game::state = GAME_WIN_STATE;
            Player::mainPlayer->introDone = false;
            Odin::mainOdin->introDone = false;
            getParent()->addChild(new WinScreen());
        }
        
        if(m_ground->getTweens().empty())
        {
            m_ground->addTween(new PositionTween2D(Vector2f(GROUND_POS_X,_GROUND_POS_Y_(true)),1.0f));
        }
    }
    
    if(Game::state == GAME_FALL_INTRO_STATE)
        m_background->startScrolling();
}

void BossfightScreen::destroyGround()
{
    for(int x = 0;x<GROUND_DESTROY_PARTS_X;x++)
    {
        for(int y = 0;y<GROUND_DESTROY_PARTS_Y;y++)
        {
            Sprite2D* part = new Sprite2D(m_ground->getTexture());
            addChild(part);
            part->setDrawSize(Vector2f(GROUND_WIDTH / GROUND_DESTROY_PARTS_X,GROUND_HEIGHT / GROUND_DESTROY_PARTS_Y));
            part->setPosition(m_ground->getPosition().x + (GROUND_WIDTH / (float)GROUND_DESTROY_PARTS_X) * x,m_ground->getPosition().y + (GROUND_HEIGHT/GROUND_DESTROY_PARTS_Y)*y);
            part->addTween(new RotationTween2D((float)getRand(200,300)));
            if((float)x/(float)GROUND_DESTROY_PARTS_X > 0.5f)
                part->addTween(new PositionTween2D(Vector2f(m_mainClass->getNativeRes().x + getRand(1000,2000),m_mainClass->getNativeRes().y/2.0f + getRand(-200,200)),2.0f));
            else
                part->addTween(new PositionTween2D(Vector2f((float)-getRand(1000,2000),m_mainClass->getNativeRes().y/2.0f + getRand(-200,200)),2.0f));
            m_groundDestroyParts.push_back(part);
            
            part->setSrcRegion(TextureRegion((GLint)((float)x * (GROUND_WIDTH / GROUND_DESTROY_PARTS_X) / 4.0f),
										     (GLint)((float)y * (GROUND_HEIGHT / GROUND_DESTROY_PARTS_Y)/ 4.0f),
											 (GLint)(GROUND_WIDTH/(float)GROUND_DESTROY_PARTS_X         / 4.0f),
											 (GLint)(GROUND_HEIGHT/(float)GROUND_DESTROY_PARTS_Y        / 4.0f)));
            part->setDepth(GROUND_DEPTH);
        }   
    }
    
    removeChild(m_ground,true,true);
    m_ground = nullptr;
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
}

void BossfightScreen::addBoundaries()
{
    int random = getRand(0,2);
    
    m_ground = new Sprite2D(random == 0 ? "ground.png" : (random == 1 ? "ground1.png" : "ground2.png"));
    
    addChild(m_ground);
    m_ground->setPosition(GROUND_POS_X,_GROUND_POS_Y_(true));
    m_ground->setScale(GAME_SCALE,GAME_SCALE);
    
    m_ground->setName("Ground");
    m_ground->setDepth(GROUND_DEPTH);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
}

void BossfightScreen::spawnObstacle()
{
    int random = getRand(0,2);
    
    Sprite2D* obstacle = new Sprite2D(random == 0 ? "obstacle.png" : (random == 1 ? "obstacle1.png" : "obstacle2.png") );
    addChild(obstacle);
    obstacle->setScale(GAME_SCALE,GAME_SCALE);
    
    m_obstacles.push_back(obstacle);
    
    obstacle->setDepth(OBSTACLE_DEPTH);

    float randPos = ((float)getRand((int)(RAND_PRECISION*OBSTACLE_MIN_X),(int)(RAND_PRECISION*OBSTACLE_MAX_X)))/RAND_PRECISION;
    
    obstacle->setPosition(randPos,OBSTACLE_Y_START_POS);
    obstacle->addTween(new PositionTween2D(Vector2f(randPos,OBSTACLE_Y_END_POS),OBSTACLE_MOVE_TIME));
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
}

void BossfightScreen::spawnArrowPickup()
{
    Vector2f spawnPos;
    Sprite2D* arrowPickup = nullptr;
    
    arrowPickup = new Sprite2D(m_mainClass->getResourceManager()->loadTexture("arrowPickUp.png",Settings::geti(SPRITE2D_FILTERING)));
    arrowPickup->setScale(GAME_SCALE,GAME_SCALE);
    addChild(arrowPickup);
    
    
    if(Game::state == GAME_BOSS_GROUND_STATE)
    {
        spawnPos.y = ARROW_PICKUP_GROUND_SPAWN_Y_POS;
        spawnPos.x = ((float)getRand((int)(RAND_PRECISION*ARROW_PICKUP_GROUND_MIN_X),(int)(RAND_PRECISION*ARROW_PICKUP_GROUND_MAX_X)))/RAND_PRECISION;
    }
        
    else if(Game::state == GAME_BOSS_FALL_STATE)
    {
        spawnPos.y = ARROW_PICKUP_FALL_SPAWN_Y_POS;
        spawnPos.x = ((float)getRand((int)(RAND_PRECISION*ARROW_PICKUP_FALL_MIN_X),(int)(RAND_PRECISION*ARROW_PICKUP_FALL_MAX_X)))/RAND_PRECISION;
    }
        
    
    arrowPickup->setPosition(spawnPos);
    
    if(Game::state == GAME_BOSS_GROUND_STATE)
    {
        arrowPickup->addTween(new PositionTween2D(Vector2f(arrowPickup->getPosition().x,GROUND_POS_Y-ARROW_PICKUP_HEIGHT),ARROW_PICKUP_GROUND_MOVE_TIME));
    }
    else if(Game::state == GAME_BOSS_FALL_STATE)
    {
        arrowPickup->addTween(new PositionTween2D(Vector2f(arrowPickup->getPosition().x,ARROW_PICKUP_FALL_Y_END_POS),ARROW_PICKUP_FALL_MOVE_TIME));
    }
    
    arrowPickup->setDepth(ARROW_PICKUP_DEPTH);
    
    m_arrowPickups.push_back(arrowPickup);
    m_arrowPickupTimes.push_back(ARROW_PICKUP_LAY_TIME);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
}