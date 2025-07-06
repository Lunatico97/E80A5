#include <gui.hpp>

GUI::GUI(MMU& mmui): mmu(mmui)
{
    rndr = new Renderer("E80A5", SCRW, SCRH);
    sys_font = rndr->loadFont("rsrc/font.ttf", FTPT);
    sys_text = rndr->loadText("E80A5", sys_font, CLR_CYAN);
    flag_text = rndr->loadText("S  Z  x  AC  x  P  x  CY", sys_font, CLR_GREEN);
    _active = true;    
}

GUI::~GUI()
{
    rndr->freeTex(flag_text);
    rndr->freeTex(sys_text);
    rndr->freeFont(sys_font);
    rndr->cleanRenderer();
    delete rndr;
}

void GUI::draw_instr()
{
    const std::string raw_str = Utils::readTextFromFile("bin/counter3.e8");
    std::stringstream ss(raw_str);
    std::string line;
    int line_num = 0;

    while (std::getline(ss, line)) 
    {
        if(!line.empty())
        {
            reg_text = rndr->loadText(line.c_str(), sys_font, CLR_WHITE);
            rndr->render(20, 50+line_num*(FTPT), reg_text);
            rndr->freeTex(reg_text);
            line_num++;
        }
    }
}

void GUI::draw_psw()
{
    // S Z x AC x P x CY
    rndr->render(475, 5, flag_text);

    SDL_Rect rect = {475, 30, 15, 15};
    for(u8 i=0; i<8; i++)
    {
        u8 bit = (mmu.tapU8(F) & (0x80 >> i)) >> i;
        if(i > 0 && i%2 == 0) rndr->renderRect(rect, 128, 128, 128, bit);
        else rndr->renderRect(rect, 255, 0, 0, bit);
        rect.x += 20;
    }
}

void GUI::draw_reg_stack()
{
    // ACC
    std::string text;
    text = Utils::logU8("ACC", mmu.tapU8(A));
    reg_text = rndr->loadText(text.c_str(), sys_font, CLR_YELLOW);
    rndr->render(500, 75, reg_text);
    rndr->freeTex(reg_text);

    // GPRs
    const char* registers[] = { "A", "F", "B", "C", "D", "E", "H", "L" };
    for(u8 i=2; i<8; i++)
    {
        text = Utils::logU8(registers[i], mmu.tapU8((REG)i));
        reg_text = rndr->loadText(text.c_str(), sys_font, CLR_BLUE);
        rndr->render(500, 100+(i-2)*(FTPT), reg_text);
        rndr->freeTex(reg_text);
    }

    // SPRs
    const char* offsets[] = { "AF", "BC", "DE", "HL", "SP", "PC", "WZ" };
    for(u8 i=4; i<7; i++)
    {
        text = Utils::logU16(offsets[i], mmu.tapU16((OFF)i));
        reg_text = (i != 6) ? rndr->loadText(text.c_str(), sys_font,  CLR_MAGENTA) :
                              rndr->loadText(text.c_str(), sys_font,  CLR_GREY);
        rndr->render(550, 100+(i-4)*(FTPT), reg_text);
        rndr->freeTex(reg_text);
    }
}

void GUI::run_gui()
{
    while(_active)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) _active = false;
        }

        rndr->setColor(0, 0, 0, 255);
        rndr->clear();
        rndr->render(5, 5, sys_text);
        draw_psw();
        draw_reg_stack();
        draw_instr();
        rndr->display();
    }
}