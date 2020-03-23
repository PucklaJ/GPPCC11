#ifndef ARROW_H
#define ARROW_H
#include <Sprite2D.h>

using namespace Johnny;

class Arrow : public Sprite2D
{
public:
    Arrow(const Vector2f& position,const Vector2f& direction);
    ~Arrow();
    
    bool init() override;
    bool update() override;
    void quit() override;
private:
    void controlPhysics();
    void checkOdin();

    Vector2f m_velocity;
};


#endif