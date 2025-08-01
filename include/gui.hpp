#include <renderer.hpp>
#include <mmu.hpp>

#ifndef __GUI_H__
#define __GUI_H__

#define SCRW 640
#define SCRH 480
#define FTPT 20
#define CLR_RED {255, 0, 0, 255}
#define CLR_GREEN {0, 255, 0, 255}
#define CLR_BLUE {0, 0, 255, 255}
#define CLR_CYAN {0, 255, 255, 255}
#define CLR_YELLOW {255, 255, 0, 255} 
#define CLR_MAGENTA {255, 0, 255, 255}
#define CLR_WHITE {255, 255, 255, 255}
#define CLR_GREY {127, 127, 127, 255}

class GUI
{
    public:
        GUI(MMU& mmu);
        ~GUI();

        void run_gui();

    private:
        void draw_instr();
        void draw_reg_stack();
        void draw_psw();

        MMU& mmu;
        bool _active;
        Renderer* rndr;
        SDL_Event event;
        TTF_Font* sys_font;
        SDL_Texture *sys_text, *flag_text, *reg_text;
};

#endif