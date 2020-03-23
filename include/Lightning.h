#ifndef LIGHTNING_H
#define LIGHTNING_H
#include <Sprite2D.h>


namespace Johnny
{
    class Texture;
}

using namespace Johnny;

class Lightning : public Sprite2D
{
public:
    Lightning(const Vector2f& position);
    ~Lightning();
    
    bool init() override;
    bool update() override;
    void quit() override;
private:
    static Texture* m_lightning;
    static Texture* m_lightning1;
    static Texture* m_lightning2;
    static Texture* m_lightning_big;
    static Texture* m_lightning_big1;
    static Texture* m_lightning_big2;

    void controlPhysics();
    void controlHitPlayer();
    void checkGettingBig();

    Vector2f m_velocity;
    bool m_big = false;
    float m_time = 0.0f;
};

#endif