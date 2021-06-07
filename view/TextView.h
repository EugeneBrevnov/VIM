
#ifndef VIM_TEXTVIEW_H
#define VIM_TEXTVIEW_H

#include <curses.h>
#include "../model/Model.h"
#include "GuiWidget.h"

class TextView : public GuiWidget {
public:
    TextView();

    void setRect(size_t nrows, size_t ncols, size_t y0, size_t x0, Model* model);

    void onUpdate() override;

    void setCursorTextPosition(size_t line, size_t col);

    void replaceModel(Model *pModel);

private:
    void redrawAll();

    WINDOW *m_window;
    Model *m_model;
    int64_t width, height;
    int64_t m_window_line, m_window_col;
    int64_t m_cursor_line, m_cursor_col;
};

#endif //VIM_TEXTVIEW_H
