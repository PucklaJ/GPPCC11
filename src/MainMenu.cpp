#include "../include/MainMenu.h"
#include "../include/BossfightScreen.h"
#include "../include/Defines.h"
#include "../include/Game.h"
#include "../include/Player.h"
#include "../include/Odin.h"
#include "../include/Sounds.h"
#include <Johnny.h>

MainMenu::MainMenu() : Actor("MainMenu")
{
    m_visible = false;
}

MainMenu::~MainMenu()
{
    
}

bool MainMenu::init()
{
    Settings::seti(SPRITE2D_FILTERING,GL_LINEAR);
    
    Actor::init();
    
    TTF_Font* font = m_mainClass->getResourceManager()->loadFont("FallingSky.otf",48);
    SDL_Color col;
    col.r = Colors::BLACK.r;
    col.g = Colors::BLACK.g;
    col.b = Colors::BLACK.b;
    col.a = Colors::BLACK.a;
    m_text = new TextActor2D(font,col,"Press Enter or Start to play");
    addChild(m_text);
    m_text->setDepth(HUD_DEPTH);
    m_text->setPosition(m_mainClass->getNativeRes()/2.0f - m_text->getActualSize()/2.0f);
    
    m_mainClass->getJoystickManager()->getListener()->addListener(this,0);
    
    m_game = static_cast<Game*>(m_mainClass);
    
    Settings::seti(SPRITE2D_FILTERING,GL_LINEAR);
    
    Sounds::introTheme->play2d(true);
    
    return true;
}

bool MainMenu::update()
{
    if(transitionToBoss || m_mainClass->getInputManager()->justPressed(Keys::RETURN))
    {
        transitionToBossFight();
    }
    
    return true;
}

void MainMenu::quit()
{
    detach();
    Actor::quit();
}

void MainMenu::onButtonDown(const ControllerButtonEvent &e)
{
    if((unsigned int)e.button == Buttons::START)
    {
        transitionToBoss = true;
    }
}

void MainMenu::transitionToBossFight()
{
    m_game->getBossfightScreen()->setPlayer(new Player());
    m_game->getBossfightScreen()->addChild(m_game->getBossfightScreen()->getPlayer());
    getParent()->removeChild(this);
    
    Game::state = GAME_INTRO_STATE;
}