

#include "Window.h"

Window::Window() : m_state(NORMAL), m_controller{}, m_model{} {};

 void Window::run(Controller* controller, Model* model) {
     m_controller = controller;
     initscr();
     noecho();
     nodelay(stdscr, true);
     keypad(stdscr, TRUE);
     m_stateTextField.setRect(100, LINES - 2, 1);
     m_commandTextField.setRect(COLS, LINES - 1, 0);
     m_textView.setRect(LINES - 2, COLS, 0, 0, model);
     changeState(NORMAL);
     //refresh();
     while (!m_finished) {
         int c = getch();
         // if (c == 'q') { break; }
         if (c == 27) getch();
         if (c != ERR) {
             if (m_state == NORMAL || m_state == INPUT || m_state == HELP) {
                 m_controller->input(c);
             }
             else if (m_state == COMMAND || m_state == SEARCH) {
                 if (c == '\n' || c == KEY_ENTER) {
                     m_controller->wide_input(m_commandTextField.getText());
                     m_commandTextField.setText("");
                 }
                 else if (c == 27) {
                     m_commandTextField.setText("");
                     m_controller->input(c);
                 }
                 else {
                     m_commandTextField.userInput(c);
                     if (m_commandTextField.getText().empty()) {
                         m_controller->input(27);
                     }
                 }
             }
         }
         std::map<State, std::string> text = {
            {NORMAL,  "NORMAL"},
            {INPUT,   "INPUT"},
            {SEARCH,  "SEARCH"},
            {HELP,  "HELP"},
            {COMMAND, "COMMAND"},
         };
         m_stateTextField.setText(text[m_state] + "               " + model->m_filename + "               "
             + std::to_string(m_controller->cursor_row + 1) + "/" + std::to_string(model->n_lines()));
         //m_commandTextField.setText(m_command);
         m_commandTextField.onUpdate();
         m_stateTextField.onUpdate();
         m_textView.onUpdate();
         doupdate();
     }
     endwin();
 }

 void Window::changeState(State newState) {
     m_state = newState;
     if (newState == EXIT) {
         m_finished = true;
     }
     std::map<State, std::string> text = {
             {NORMAL,  "NORMAL"},
             {INPUT,   "INPUT"},
             {SEARCH,  "SEARCH"},
             {HELP,  "HELP"},
             {COMMAND, "COMMAND"},
     };
     std::string command;
     if (m_state == SEARCH) {
         command = "/";
     }
     else if (m_state == COMMAND) {
         command = ":";
     }
     m_commandTextField.setText(command);
     m_stateTextField.setText(text[newState]);
 }

 void Window::setCursosPos(size_t row, size_t column) {
     m_textView.setCursorTextPosition(row, column);
 }

 void Window::modelReplaced(Model* pModel) {
     m_model = pModel;
     m_textView.replaceModel(pModel);
 }