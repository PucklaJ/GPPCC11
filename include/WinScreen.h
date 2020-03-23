#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H
#include <Actor.h>
#include <JoystickListener.h>

namespace Johnny
{
    class TextActor2D;
}

using namespace Johnny;

class WinScreen : public Actor, public AxisButtonListener
{
public:
    WinScreen();
    ~WinScreen();
    
    bool init() override;
    bool update() override;
    void quit() override;
    
    void onAxisMotion(const ControllerAxisEvent &e) override;
    void onButtonDown(const ControllerButtonEvent &e) override;
    void onButtonUp(const ControllerButtonEvent &e) override;
    
private:
    void transitionToBossFight();
    bool transitionToBoss = false;

    TextActor2D* m_winText = nullptr;
    TextActor2D* m_winPressText = nullptr;
};


#endif