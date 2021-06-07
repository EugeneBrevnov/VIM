

#include "TextField.h"

TextField::TextField(size_t nlines, size_t ncols, size_t y0, size_t x0) : m_width(), m_window(newwin(nlines, ncols, y0, x0)) { };


void TextField::setRect(size_t ncols, size_t y0, size_t x0) {
    m_window = newwin(1, ncols, y0, x0);
    m_width = ncols;
}

void TextField::userInput(int c) {
    if (c == KEY_BACKSPACE || c == 0x8) {
        if (m_text.size() >= 2) {
            m_text.pop_back();
        }
    }
    else {
        m_text.push_back(c);
    }
}

void TextField::setText(std::string text) {
    m_text = std::move(text);
}

void TextField::onUpdate() {
    werase(m_window);
    wprintw(m_window, "%-*s", m_width, m_text.c_str());
    wnoutrefresh(m_window);
}

std::string const& TextField::getText() {
    return m_text;
}