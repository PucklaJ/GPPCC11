#include "../include/Game.h"
#ifdef __MACOSX__
#include <unistd.h>

void changeDirectory(char* argv[])
{
    std::string arg = argv[0];
    unsigned long pos = -1;
    
    for(unsigned long i = arg.length()-1;i>0;i--)
    {
        if(arg[i] == '/')
        {
            pos = i;
            break;
        }
    }
    
    std::string dir = "";
    dir += arg.substr(0,pos);
    
    std::cout << "Changing directory: " << dir << std::endl;
    
    if(chdir(dir.c_str())<0)
    {
        std::cout << "Change Directory error" << std::endl;
    }
    
}
#endif

#ifdef _WIN32
int SDL_main(int args,char* argv[])
#else
int main(int args, char* argv[])
#endif
{
#ifdef __MACOSX__
    changeDirectory(argv);
#endif
    Game().run();
    
    return 0;
}