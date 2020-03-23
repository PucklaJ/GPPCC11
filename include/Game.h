#pragma once
#include <MainClass.h>
#include <JoystickListener.h>
#include <cAudio/cAudio.h>

using namespace Johnny;

class BossfightScreen;

class Game : public MainClass, public AxisButtonListener
{
public:
    static int state;
    static int phase;

	Game();
	~Game();

	bool init() override;
	bool update() override;
	bool render() override;
	void quit() override;
    
    void onAxisMotion(const ControllerAxisEvent &e) override;
    void onButtonDown(const ControllerButtonEvent &e) override;
    void onButtonUp(const ControllerButtonEvent &e) override;
    
    BossfightScreen* getBossfightScreen() {return m_bossfightScreen;}
    cAudio::IAudioManager* audioManager = nullptr;
    
private:
    BossfightScreen* m_bossfightScreen = nullptr;
    
};

