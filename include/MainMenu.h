#ifndef MAINMENU_SCREEN_H
#define MAINMENU_SCREEN_H
#include <Actor.h>
#include <TextActor2D.h>
#include <JoystickListener.h>

using namespace Johnny;

class Game;

class MainMenu : public Actor,public AxisButtonListener
{
public:
    MainMenu();
    ~MainMenu();
    
    bool init() override;
    bool update() override;
    void quit() override;
    
    void onAxisMotion(const ControllerAxisEvent &e) override {}
    void onButtonDown(const ControllerButtonEvent &e) override;
    void onButtonUp(const ControllerButtonEvent &e) override {}
    
private:
    void transitionToBossFight();
    bool transitionToBoss = false;

    TextActor2D* m_text = nullptr;
    Game* m_game = nullptr;
};


#endif