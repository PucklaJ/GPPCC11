#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <Actor.h>

namespace Johnny
{
    class Sprite2D;
    class Texture;
}

using namespace Johnny;

class Background : public Actor
{
public:
    Background();
    ~Background();
    
    bool init() override;
    bool update() override;
    void quit() override;
    
    void startScrolling();
    void stopScrolling();
    
    bool canSpawnObstacles() const {return m_canSpawnObstacles;}
    
private:
    void setNewBackground();
    short getRandomBackgroundIndex(short notcheck);

    Sprite2D* m_backgrounds[3];
    Texture* m_backgroundTex[3];
    short m_backgroundIndex[3];
    short m_middleBackground = 0;
    bool m_scrolling = true;
    bool m_canSpawnObstacles = true;
};


#endif