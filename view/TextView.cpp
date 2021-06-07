
#include <iostream>
#include "TextView.h"

void TextView::redrawAll() {
    werase(m_window);
    std::cout << m_window_col << " " << m_cursor_col << std::endl;
    size_t i;
    for ( i = 0; i < height && m_window_line + i < m_model->n_lines(); ++i) {
        
       
        std::string const &line = m_model->line(m_window_line + i).c_str();
        if (m_window_col < line.size()) {
            mvwprintw(m_window, i, 0, "%.*s",
                    //m_cursor_col, m_window_col, width,
                    //m_cursor_line, m_window_line, height,
                      width,
                      &line[m_window_col]);

        }
        
    }
    char a='_';
    if (m_model->n_lines())
    {
        if (m_model->lineSize(m_cursor_line)> m_cursor_col)
        {
            std::string const& line = m_model->line(m_cursor_line).c_str();
            a = line[m_cursor_col];
        }
    }
    mvwaddch(m_window, m_cursor_line - m_window_line, m_cursor_col - m_window_col, a | A_BOLD | A_UNDERLINE);
    wnoutrefresh(m_window);
}


void TextView::setCursorTextPosition(size_t line, size_t col) {
    m_cursor_line = line;
    m_cursor_col = col;
    if (m_cursor_line == m_window_line + height) {
        m_window_line++;
    } else if (m_cursor_line > m_window_line + height) {
        m_window_line = std::max<int64_t>(m_cursor_line - height / 2, 0);
    }
    if (m_cursor_line == m_window_line - 1) {
        m_window_line--;
    } else if (m_cursor_line < m_window_line - 1) {
        m_window_line = std::max<int64_t>(m_cursor_line - height / 2, 0);
    }
    if (m_cursor_col == m_window_col + width) {
        m_window_col++;
    } else if (m_cursor_col > m_window_col + width) {
        m_window_col = std::max<int64_t>(m_cursor_col - width / 2, 0);
    }
    if (m_cursor_col == m_window_col - 1) {
        m_window_col--;
    } else if (m_cursor_col < m_window_col - 1) {
        m_window_col = std::max<int64_t>(m_cursor_col - width / 2, 0);
    }
}

void TextView::replaceModel(Model *pModel) {
    m_model = pModel;
}

TextView::TextView()
    : m_window{}, m_model{}, width{}, height{}, m_window_line{}, m_window_col{}, m_cursor_line{},
    m_cursor_col{} {}

void TextView::setRect(size_t nrows, size_t ncols, size_t y0, size_t x0, Model* model) {
    m_window = newwin(nrows, ncols, y0, x0);
    width = ncols;
    height = nrows;
    m_model = model;
    m_window_line = m_window_col = 0;
    m_cursor_col = m_cursor_line = 0;
}


void TextView::onUpdate() {
    redrawAll();
}