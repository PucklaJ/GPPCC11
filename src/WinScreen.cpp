#include "../include/WinScreen.h"
#include "../include/BossfightScreen.h"
#include "../include/Game.h"
#include "../include/Defines.h"
#include "../include/Sounds.h"
#include <Johnny.h>

WinScreen::WinScreen() : Actor("WinScreen")
{
    
}

WinScreen::~WinScreen()
{
    
}

bool WinScreen::init()
{
    Settings::seti(SPRITE2D_FILTERING,GL_LINEAR);
    
    SDL_Color white = {0,0,0,255};
    TTF_Font* font = m_mainClass->getResourceManager()->loadFont("FallingSky.otf",48);
    
    m_winText = new TextActor2D(font,white,"You won");
    m_winPressText = new TextActor2D(font,white,"Press Enter or Start to play again");
    
    addChild(m_winText);
    addChild(m_winPressText);
    
    m_winPressText->setScale(0.5f,0.5f);
    
    m_winText->setPosition(m_mainClass->getNativeRes().width/2.0f - m_winText->getActualSize().width/2.0f,m_mainClass->getNativeRes().height/2.0f - m_winText->getActualSize().height/2.0f);
    m_winPressText->setPosition(m_mainClass->getNativeRes().width/2.0f - m_winPressText->getActualSize().width/2.0f,m_winText->getPosition().y + m_winText->getActualSize().height + 30.0f);
    
    m_winPressText->addTween(new BlinkTween2D(0.5f));
    
    m_mainClass->getJoystickManager()->getListener()->addListener(this,0);
    
    m_winText->setDepth(HUD_DEPTH);
    m_winPressText->setDepth(HUD_DEPTH);
    
    Settings::seti(SPRITE2D_FILTERING,GL_NEAREST);
    
    Sounds::mainTheme->stop();
    Sounds::introTheme->play2d(true);
    
    return true;
}

bool WinScreen::update()
{
    if(transitionToBoss || m_mainClass->getInputManager()->justPressed(Keys::RETURN))
    {
        transitionToBossFight();
    }
    
    return true;
}

void WinScreen::quit()
{
    detach();
}

void WinScreen::onAxisMotion(const ControllerAxisEvent &e)
{
    
}

void WinScreen::onButtonDown(const ControllerButtonEvent &e)
{
    if((unsigned int)e.button == Buttons::START)
    {
        transitionToBoss = true;
    }
}

void WinScreen::onButtonUp(const ControllerButtonEvent &e)
{
    
}

void WinScreen::transitionToBossFight()
{
    getParent()->removeChild(BossfightScreen::mainBossfightScreen,true,true);
    
    getParent()->removeChild(this);
    getParent()->addChild(new BossfightScreen(),true);
    BossfightScreen::mainBossfightScreen->setPlayer(new Player());
    BossfightScreen::mainBossfightScreen->addChild(BossfightScreen::mainBossfightScreen->getPlayer());
    Game::state = GAME_INTRO_STATE;
}