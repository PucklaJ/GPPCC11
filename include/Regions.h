#ifndef REGIONS_H
#define REGIONS_H
#include <Geometry.h>

namespace Regions
{
    namespace Player
    {
        namespace Ground
        {
            namespace Stand
            {
                const TextureRegion stand(0,0,20,25);
            }
            
            namespace Run
            {
                const TextureRegion run1(20,0,20,25);
                const TextureRegion run15(40,0,20,25);
                const TextureRegion run2(78,100,20,25);
                const TextureRegion run25(60,0,20,25);
            }
            
            namespace Jump
            {
                namespace Up
                {
                    const TextureRegion up1(120,25,20,25);
                    const TextureRegion up2(160,25,20,25);
                    const TextureRegion up3(180,25,20,25);
                }
                
                namespace Down
                {
                    const TextureRegion down1(140,25,20,25);
                    const TextureRegion down2(0,50,20,25);
                    const TextureRegion down3(20,50,20,25);
                }
            }
            
            namespace Sword
            {
                const TextureRegion sword1(40,50,38,25);
                const TextureRegion sword2(78,50,38,25);
                const TextureRegion sword3(116,50,38,25);
            }
            
            namespace Knockback
            {
                const TextureRegion knockback(38,100,20,25);
            }
        }
        
        namespace Fall
        {
            namespace Down
            {
                const TextureRegion down1(80,0,20,25);
                const TextureRegion down2(100,0,20,25);
                const TextureRegion down3(120,0,20,25);
            }
            
            namespace Right
            {
                const TextureRegion right1(140,0,20,25);
                const TextureRegion right2(180,0,20,25);
                const TextureRegion right3(0,25,20,25);
            }
            
            namespace Left
            {
                const TextureRegion left1(160,0,20,25);
                const TextureRegion left2(20,25,20,25);
                const TextureRegion left3(40,25,20,25);
            }
            
            namespace Up
            {
                const TextureRegion up1(60,25,20,25);
                const TextureRegion up2(80,25,20,25);
                const TextureRegion up3(100,25,20,25);
            }
            
            namespace Sword
            {
                const TextureRegion sword1(154,50,38,25);
                const TextureRegion sword2(0,75,38,25);
                const TextureRegion sword3(38,75,38,25);
            }
            
            namespace Knockback
            {
                const TextureRegion knockback(58,100,20,25);
            }
        }
        
        namespace Bow
        {
            const TextureRegion bow1(76,75,38,25);
            const TextureRegion bow2(114,75,38,25);
            const TextureRegion bow3(152,75,38,25);
            const TextureRegion bow_finish(0,100,38,25);
        }
    }
}

#endif