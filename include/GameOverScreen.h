#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H
#include <Actor.h>
#include <JoystickListener.h>

namespace Johnny
{
    class TextActor2D;
}

using namespace Johnny;

class GameOverScreen : public Actor, public AxisButtonListener
{
public:
    GameOverScreen();
    ~GameOverScreen();
    
    bool init() override;
    bool update() override;
    void quit() override;
    
    void onAxisMotion(const ControllerAxisEvent &e) override;
    void onButtonDown(const ControllerButtonEvent &e) override;
    void onButtonUp(const ControllerButtonEvent &e) override;
    
private:
    void transitionToBossFight();
    bool transitionToBoss = false;

    TextActor2D* m_gameOverText = nullptr;
    TextActor2D* m_restartText = nullptr;
    
};


#endif