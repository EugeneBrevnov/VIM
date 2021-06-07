#include "Controller.h"

State Controller::CommandMode::handle(int code) {
    if (code == 27 || code == KEY_ENTER) {
        return NORMAL;
    }
    assert(false);
    return COMMAND;
}

std::string getCommandName(std::string const &s) {
    assert(s[0] == ':');
    auto end = std::find_if(s.begin(), s.end(), isspace);
    return std::string(s.begin() + 1, end);
}

std::string getCommandArgument(std::string const &s) {
    assert(s[0] == ':');
    auto beg = std::find_if(s.begin(), s.end(), isspace);
    beg = std::find_if_not(beg, s.end(), isspace);
    auto end = std::find_if(beg, s.end(), isspace);
    return std::string(beg, end);
}

State Controller::CommandMode::wide_handle(std::string s) {
    std::string commandName = getCommandName(s);
    if (commandName == "o") {
        std::string arg = getCommandArgument(s);
        if (arg.empty()) {
            throw std::runtime_error("bad filename");
        }
        c->m_model->open(arg);
        c->m_textExists = true;
        c->move_to(0, 0);
    }
    if (commandName == "w") {
        std::string arg = getCommandArgument(s);
        c->m_model->write(arg);
    }
    if (commandName == "x" || commandName == "wq!") {
        c->m_model->write("");
        return EXIT;
    }
    if (commandName == "q!") {
        return EXIT;
    }
    if (commandName == "q") {
        if (!c->m_model->modified()) {
            return EXIT;
        }
    }
    if (commandName == "h") {
        std::cout << "help" << std::endl;
        return HELP;
    }

    int number = 0;
    bool isLineNumber = true;
    try {
        number = std::stoi(commandName) - 1;
    } catch (...) {
        isLineNumber = false;
    }
    isLineNumber &= number > 0;
    if (isLineNumber) {
        c->cursor_row = number;
        return NORMAL;
    }

    return NORMAL;
}
