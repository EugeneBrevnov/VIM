#include <map>
#include <functional>
#include <string>
#include <sstream>
#include "Controller.h"

static const char* helpText =
        "UP,DOWN,LEFT,RIGHT - Move the cursor by 1 position\n"
        "q    - Exit\n"
        "\n"
        "Usage: vim\n"
    "RIGHT Move the cursor to the right by 1 position\n"
    "LEFT Move the cursor to the left by 1 position\n"
    "UP Move the cursor up 1 line (the cursor position must be preserved, or move to the end of the line if the line is shorter)\n"
    "DOWN Moves the cursor down by 1 line (the cursor position must be preserved, or move to the end of the line if the line is shorter)\n"
    "^ Move the cursor to the beginning of the line\n"
    "$ Move cursor to end of line\n"
    "w Move the cursor to the end of the word to the right of the cursor (if the cursor is at the end of the word, then move the cursor to the end of the next word)\n"
    "b moves the cursor to the beginning of the word left of the cursor (if the cursor is at the beginning of words, then move the cursor to the beginning of the previous word) \n"
    "gg move to the beginning of the file\n"
    "G Go to end of file\n"
    "NG go to line number N. For example, 10G-jump to the 10th line\n"
    "PU Go to Screen up\n"
    "PD go to the screen down\n"
    "dd Delete characters from the current cursor position to the end of the line\n"
    "x Delete the character after the cursor \n"
    "dw Delete the word under the cursor, including the space on the right.\n"
    "dd Cut current line \n"
    "d^ Delete characters from the beginning of the line to the current cursor position \n"

    "d$ Delete characters from the current cursor position to the end of the line\n"

    "yy Copy current line\n"
    "yw Copy word under cursor \n"
    "y^ Copy characters from the beginning of the line to the current cursor position\n"
    "y$ Copy characters from the current cursor position to the end of the line\n"
    "p Paste after cursor\n"
    "\n"
    "/text<CR> Enter the search text\n"
    "?text<CR> Enter the search text\n"
    "n Repeat the search in the down direction\n"
    "N Repeat the search in the up direction.\n"
    "\n"
    "i Enter text before the cursor\n"
    "I Go to beginning of line and start entering text\n"
    "A Go to end of line and start entering text\n"
    "o Create a line at the top and start typing\n"
    "O Create a line from the bottom and start typing\n"
    "S Delete the contents of the row and begin entering text\n"
    "r Replace one character under the cursor\n"
    "\n"
    "o filename Open file filename\n"
    "x Write to current file and exit\n"
    "w Write to current file\n"
    "w filename Write to filename\n"
    "q Exit. If the file has been changed, then the output is only possible via q!\n"
    "q! To exit without saving\n"
    "wq! Write to the current file and exit\n"
    "number Jump to the string number\n"
    "h Display help for commands\n";

void Controller::HelpMode::init() {
    if (!m_helpModel) {
        m_helpModel = std::make_unique<Model>(Model());
    }
    std::istringstream helpTextIn(helpText);
    m_helpModel->read(helpTextIn);
    m_originalModel = c->m_model;
    c->replaceModel(m_helpModel.get());
   /// c->m_model = m_helpModel.get();
}

void Controller::HelpMode::deinit() {
    c->replaceModel(m_originalModel);
}

State Controller::HelpMode::handle(int code) {
    switch (code) {
    case KEY_DOWN:
        c->move_delta(+1, 0); break;
    case KEY_UP:
        c->move_delta(-1, 0); break;
    case KEY_LEFT:
        c->move_delta(0, -1); break;
    case KEY_RIGHT:
        c->move_delta(0, +1); break;
    case 'q':
        return NORMAL;
    default:
        break;
    }
    return HELP;
}

State Controller::HelpMode::wide_handle(std::string s) {
    assert(false);
    return HELP;
}
