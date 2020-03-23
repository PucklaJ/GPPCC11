#include "../include/Game.h"
#include "../include/MainMenu.h"
#include "../include/Defines.h"
#include "../include/BossfightScreen.h"
#include "../include/Sounds.h"
#include <Johnny.h>

int Game::state = GAME_MENU_STATE;
int Game::phase = 1;

Game::Game() : MainClass(InitFlags::INIT_2D |
                         InitFlags::JOYSTICK,"GPPCC11",1280,720,
						 FlagsWindow::Resizeable)
{
    Settings::seti(BACK_BUFFER_FILTERING,GL_NEAREST);
    Settings::seti(SPRITE2D_FILTERING,GL_NEAREST);
    
    srand((unsigned int)time(nullptr));
}

Game::~Game()
{
    
}

bool Game::init()
{
    audioManager = cAudio::createAudioManager(false);
    if(!audioManager)
    {
        LogManager::error("Couldn't create cAudio::IAudioManager");
    }
    else
    {
        cAudio::IAudioDeviceList* deviceList = cAudio::createAudioDeviceList();
        audioManager->initialize(deviceList->getDefaultDeviceName().c_str());
        CAUDIO_DELETE deviceList;
        
        Sounds::load(audioManager);
    }
    
    getJoystickManager()->setListener(new JoystickListener());
    getJoystickManager()->getListener()->addListener(this,0);
    
    m_bossfightScreen = new BossfightScreen();
    addChild(m_bossfightScreen);
    addChild(new MainMenu());
    
    return true;
}

bool Game::update()
{
    if(m_inputManager->justPressed(Keys::F))
    {
        m_window->setFullscreen(!m_window->isFullscreen());
    }
    else if(m_inputManager->justPressed(Keys::T))
    {
        m_window->setResolution(1280/2,720/2);
    }
    
    return true;
}

bool Game::render()
{
    return true;
}

void Game::quit()
{
    //cAudio::destroyAudioManager(audioManager);
    //audioManager = nullptr;
}

void Game::onAxisMotion(const ControllerAxisEvent &e)
{
    
}
void Game::onButtonDown(const ControllerButtonEvent &e)
{
    if((unsigned int)e.button == Buttons::SELECT)
    {
        getWindow()->setFullscreen(!getWindow()->isFullscreen());
    }
}
void Game::onButtonUp(const ControllerButtonEvent &e)
{
    
}