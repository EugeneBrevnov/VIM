#include "Controller.h"
#include <string>

State Controller::InputMode::wide_handle(std::string) {
    assert(false);
    return INPUT;
}


State Controller::InputMode::handle(int ch) {
    if (ch == 27) return NORMAL;
    if (ch == KEY_BACKSPACE || ch == 8) {
        if (c->cursor_col > 0) {
            c->m_model->erase(c->cursor_row, c->cursor_col, 1);
            c->move_delta(0, -1);
        }
    } else if (ch == KEY_ENTER || ch == 10) {

        MyString str = c->m_model->line(c->cursor_row).substr(c->cursor_col);
       
        c->m_model->erase(c->cursor_row, c->cursor_col, str.size());
        c->m_model->newLine(c->cursor_row);
        c->cursor_row++;
        c->cursor_col = 0;
        c->m_model->erase(c->cursor_row, c->cursor_col, c->m_model->lineSize(c->cursor_row));

        c->m_model->insert(c->cursor_row, 0, str);
    } else {
        if (!c->m_textExists)
        {
            c->m_model->push_back();
            c->m_textExists = true;
        }
        c->m_model->insert(c->cursor_row, c->cursor_col, std::string(1, ch));
        c->move_delta(0, 1);
    }
    return INPUT;
}
