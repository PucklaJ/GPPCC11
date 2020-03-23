#include "../include/GameOverScreen.h"
#include "../include/BossfightScreen.h"
#include "../include/Game.h"
#include "../include/Defines.h"
#include "../include/Sounds.h"
#include <Johnny.h>

GameOverScreen::GameOverScreen() : Actor("GameOverScreen")
{
    m_visible = false;
}

GameOverScreen::~GameOverScreen()
{
    
}

bool GameOverScreen::init()
{
    Settings::seti(SPRITE2D_FILTERING,GL_LINEAR);
    
    TTF_Font* font = m_mainClass->getResourceManager()->loadFont("FallingSky.otf",48);
    SDL_Color col = {0,0,0,255};
    m_gameOverText = new TextActor2D(font,col,"Game Over");
    addChild(m_gameOverText);
    m_gameOverText->setDepth(HUD_DEPTH);
    
    m_gameOverText->setPosition(m_mainClass->getNativeRes()/2.0f - m_gameOverText->getActualSize()/2.0f);
    
    m_restartText = new TextActor2D(font,col,"Press Enter or Start to try again");
    addChild(m_restartText);
    m_restartText->setDepth(HUD_DEPTH);
    m_restartText->setScale(0.5f,0.5f);
    m_restartText->setPosition(m_mainClass->getNativeRes()/2.0f - m_restartText->getActualSize()/2.0f + Vector2f(0.0f,100.0f));
    m_restartText->addTween(new BlinkTween2D(0.5f));
    
    m_mainClass->setBackgroundColor(Colors::BLACK);
    
    m_mainClass->getJoystickManager()->getListener()->addListener(this,0);
    
    m_mainClass->setBackgroundColor(Colors::LIGHT_BLUE);
    
    Settings::seti(SPRITE2D_FILTERING,GL_NEAREST);
    
    Sounds::mainTheme->stop();
    Sounds::introTheme->play2d(true);
    
    return true;
}

void GameOverScreen::onAxisMotion(const ControllerAxisEvent &e)
{
    
}
void GameOverScreen::onButtonDown(const ControllerButtonEvent &e)
{
    if((unsigned int)e.button == Buttons::START)
    {
        transitionToBoss = true;
    }
}
void GameOverScreen::onButtonUp(const ControllerButtonEvent &e)
{
    
}

bool GameOverScreen::update()
{
    if(transitionToBoss || m_mainClass->getInputManager()->justPressed(Keys::RETURN))
    {
        transitionToBossFight();
    }
    
    return true;
}

void GameOverScreen::quit()
{
    detach();
    Actor::quit();
}

void GameOverScreen::transitionToBossFight()
{
    getParent()->removeChild(BossfightScreen::mainBossfightScreen,true,true);
    
    getParent()->removeChild(this);
    getParent()->addChild(new BossfightScreen(),true);
    BossfightScreen::mainBossfightScreen->setPlayer(new Player());
    BossfightScreen::mainBossfightScreen->addChild(BossfightScreen::mainBossfightScreen->getPlayer());
    Game::state = GAME_INTRO_STATE;
}