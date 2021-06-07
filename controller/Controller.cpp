
#include "Controller.h"

void Controller::changeState(State state) {
    if (m_mode) {
        m_mode->deinit();
    }
    if (state != EXIT) {
        m_mode = m_modes[state];
        m_state = state;
    }
    clamp_row_col();
    m_signal_cursorPosChanged.emit(cursor_row, cursor_col);
    m_signal_stateChanged.emit(state);
    if (state == EXIT) {
        m_mode = nullptr;
    }
    else {
        m_mode->init();
    }
}

void Controller::input(int c) {
    if (!m_mode) return;
    State newState = m_mode->handle(c);
    if (newState != m_state) {
        Log::get()->log(std::to_string(newState));
        changeState(newState);
        clamp_move();
    }
    clamp_move();
}

void Controller::wide_input(std::string c) {
    std::cout << c << std::endl;
    if (!m_mode) return;
    State newState = m_mode->wide_handle(c);
    if (newState != m_state) {
        changeState(newState);
    }
    clamp_move();
}

void Controller::replaceModel(Model* newModel) {
    m_model = newModel;
    m_signal_modelReplaced.emit(m_model);
}

void Controller::move_delta(int64_t delta_rows, int64_t delta_cols) {
    cursor_row += delta_rows;
    cursor_col += delta_cols;
    clamp_row_col();
    m_signal_cursorPosChanged.emit(cursor_row, cursor_col);
}


void Controller::move_to(int64_t row, int64_t col, bool wide) {
    cursor_row = row;
    cursor_col = col;
    clamp_row_col(wide);
    m_signal_cursorPosChanged.emit(cursor_row, cursor_col);
}

void Controller::clamp_move() {
    move_to(cursor_row, cursor_col);
}

void Controller::clamp_row_col(bool wide) {
    if (m_model->n_lines() == 0) {
        cursor_row = 0;
        cursor_col = 0;
        return;
    }
    bool wide_borders = wide || m_state == INPUT;
    cursor_row = std::max<int64_t>(0, std::min<int64_t>(cursor_row, static_cast<int64_t>(m_model->n_lines()) - 1));
    cursor_col = std::max<int64_t>(0, std::min<int64_t>(cursor_col,
        static_cast<int64_t>(m_model->line(cursor_row).size()) - !wide_borders));
}