
#ifndef VIM_TEXTFIELD_H
#define VIM_TEXTFIELD_H

#include "GuiWidget.h"
#include <string>
#include <curses.h>
#include <iostream>
#include <cassert>

class TextField : public GuiWidget {

public:
    TextField() { }
    TextField(size_t nlines, size_t ncols, size_t y0, size_t x0);

    void setRect(size_t ncols, size_t y0, size_t x0);

    void userInput(int c);

    void setText(std::string text);

    void onUpdate() override;

    std::string const& getText();

private:
    size_t m_width;
    std::string m_text;
    WINDOW* m_window;
};


#endif //VIM_TEXTFIELD_H
