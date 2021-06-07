
#include "Controller.h"
#include <functional>
#include <string>
#include <map>
#include <algorithm>


int isNotSpace(int c) { return !isspace(c); }

std::pair<int64_t, int64_t> findNext(Model* model, int64_t row, int64_t column, std::function<int(int)>&& predicat) {
    while (row < model->n_lines()) {
        std::string const& line = model->line(row).c_str();
        auto it = std::find_if(line.begin() + column, line.end(), predicat);
        if (it != line.end()) {
            column = it - line.begin();
            break;
        }
        if (predicat('\n')) {
            column = line.size();
            break;
        }
        row++;
        column = 0;
    }
    if (row == model->n_lines()) {
        row = row - 1;
        column = model->line(row).size() - 1;
    }
    return { row, column };
}

std::pair<int64_t, int64_t> findPrev(Model* model, int64_t row, int64_t column, std::function<int(int) >&& predicat) {
    while (row >= 0) {
        std::string const& line = model->line(row).c_str();
        auto it = std::find_if(line.rend() - column, line.rend(), predicat);
        if (it != line.rend()) {
            column = line.rend() - it - 1;
            break;
        }
        if (predicat('\n')) {
            row = row - 1;
            column = model->line(row).size();
            break;
        }
        row--;
        if (row >= 0) {
            column = model->line(row).size();
        }
    }

    if (row == -1) {
        row = 0;
        column = 0;
    }
    return { row, column };
}

std::pair<int64_t, int64_t> search(bool toBottom, Model* model, int64_t row, int64_t column, std::string const& toSearch) {
    int nextLine = toBottom ? 1 : -1;
    column += toBottom ? 1 : -1;
    while (0 <= row && row < model->n_lines()) {
        std::string const& line = model->line(row).c_str();
        int pos = 0;
        if (toBottom) {
            pos = line.find(toSearch, column);
            column = 0;
        }
        else {
            pos = line.rfind(toSearch, column);
            column = -1;
        }
        if (pos != std::string::npos) {
            return { row, pos };
        }
        row += nextLine;
    }
    if (row == -1) {
        return { 0, 0 };
    }
    if (row == model->n_lines()) {
        return { model->n_lines() - 1, 0 };
    }
    return { 0, 0 };
}

State Controller::NormalMode::handle(int ch) {
    switch (ch) {
    case '/':
        return SEARCH;
    case '?':
        return SEARCH;
    case ':':
        return COMMAND;
    case KEY_DOWN:
        c->move_delta(+1, 0); break;
    case KEY_UP:
        c->move_delta(-1, 0); break;
    case KEY_LEFT:
        c->move_delta(0, -1); break;
    case KEY_RIGHT:
        c->move_delta(0, +1); break;
    default:
        break;
    }
    std::vector<std::pair<std::string, std::function<void()>>> commands = {
            {"PU",  [&]() { if (c->cursor_row - 28 >= 0) c->move_delta(-27, 0); else c->move_delta(-c->cursor_row, 0); }},
            {"PD",  [&]() { if (c->cursor_row + 28 < c->m_model->n_lines()) c->move_delta(+27, 0); else c->move_delta(+(c->m_model->n_lines() - c->cursor_row), 0); }},
            {"d$", [&]() {
                c->m_model->erase(c->cursor_row, c->cursor_col, c->m_model->lineSize(c->cursor_row) - c->cursor_col);
            }},
             {"d^", [&]() {
                c->m_model->erase(c->cursor_row, 0, c->cursor_col);
                c->move_delta(0, -c->cursor_col);
            }},
             {"y^", [&]() {
                c->m_copypaste = c->m_model->line(c->cursor_row).substr(0,c->cursor_col);
            }},
            {
             "y$", [&]() {
                c->m_copypaste = c->m_model->line(c->cursor_row).substr(c->cursor_col);
            }},
            {"0",  [&]() { c->move_to(c->cursor_row, 0); }},
            {"^",  [&]() { c->move_to(c->cursor_row, 0); }},
            {"$",  [&]() { c->move_to(c->cursor_row, c->m_model->line(c->cursor_row).size() - 1); }},
            {"gg", [&]() { c->move_to(0, 0); }},
            {"G",  [&]() { c->move_to(c->m_model->n_lines() - 1, c->cursor_col); }},
            {"x",  [&]() { if (c->m_model->lineSize(c->cursor_row))c->m_model->erase(c->cursor_row, c->cursor_col, 1); }},
            {"X",  [&]() { if (c->m_model->lineSize(c->cursor_row) && c->cursor_col > 0)c->m_model->erase(c->cursor_row, c->cursor_col - 1, 1); c->move_delta(0, -1); }},
            {"n",  [&]() {
                auto p = search(c->m_searchDirectionToBottom, c->m_model, c->cursor_row, c->cursor_col, c->m_searchString);
                c->cursor_row = p.first;
                c->cursor_col = p.second; }},
            {"N",  [&]() {
                auto p = search(!c->m_searchDirectionToBottom, c->m_model, c->cursor_row, c->cursor_col, c->m_searchString);
                c->cursor_row = p.first;
                c->cursor_col = p.second; }},
            {"dw", [&]() {
                if (isspace(c->m_model->line(c->cursor_row)[c->cursor_col])) {
                    return;
                }
                auto beg = findPrev(c->m_model, c->cursor_row, c->cursor_col, isspace);
                beg = findNext(c->m_model, beg.first, beg.second, isNotSpace);

                auto end = findNext(c->m_model, c->cursor_row, c->cursor_col, isspace);
                end = findNext(c->m_model, end.first, end.second, isNotSpace);

                c->m_model->erase(c->cursor_row, beg.second, end.second - beg.second);
            }},
            {"yw", [&]() {
                if (isspace(c->m_model->line(c->cursor_row)[c->cursor_col])) {
                    return;
                }
                auto beg = findPrev(c->m_model, c->cursor_row, c->cursor_col, isspace);
                beg = findNext(c->m_model, beg.first, beg.second, isNotSpace);

                auto end = findNext(c->m_model, c->cursor_row, c->cursor_col, isspace);

                c->m_copypaste = c->m_model->line(c->cursor_row).substr(beg.second, end.second - beg.second);
            }},
            {"w",  [&]() {
                auto p = findNext(c->m_model, c->cursor_row, c->cursor_col, isspace);
                auto p2 = findNext(c->m_model, p.first, p.second, isNotSpace);
                c->move_to(p2.first, p2.second);
            }},
            {"b",  [&]() {
                auto p = findPrev(c->m_model, c->cursor_row, c->cursor_col, isNotSpace);
                p = findPrev(c->m_model, p.first, p.second, isspace);
                p = findNext(c->m_model, p.first, p.second, isNotSpace);
                if (p.first >= 0 && p.second >= 0)
                c->move_to(p.first, p.second);
            }},
            {"dd", [&]() {
                if (c->m_model->n_lines() > 0) {
                    if (c->m_model->n_lines() == 1)
                    c->m_textExists = false;
                    c->m_copypaste = c->m_model->line(c->cursor_row);
                    c->m_model->removeLine(c->cursor_row);
                }
            }},
            {"yy", [&]() {
                c->m_copypaste = c->m_model->line(c->cursor_row);
            }},
            {"p",  [&]() {
                c->m_model->insert(c->cursor_row, c->cursor_col, c->m_copypaste);
            }},
    };
    std::map<std::string, std::function<void()>> start_input_commands = {
            {"i", []() {}},
            {"o", [&]() {c->m_model->newLine(c->cursor_row - 1); }},
            {"O", [&]() {c->m_model->newLine(c->cursor_row + 1); c->move_delta(+1, 0);  }},
            {"I", [&]() { c->move_to(c->cursor_row, 0); }},
            {"A", [&]() { c->move_to(c->cursor_row, c->m_model->line(c->cursor_row).size(), true); }},
            {"S", [&]() { c->m_model->erase(c->cursor_row, 0, c->m_model->line(c->cursor_row).size()); }}
    };
    m_command.push_back(ch);
    // check if any suffix of m_command is correct command
    for (auto it = m_command.begin(); it != m_command.end(); ++it) {
        std::string command(it, m_command.end());
        for (auto const& com : commands) {
            std::string const& commandName = com.first;
            if (commandName.size() <= command.size() &&
                std::equal(commandName.begin(), commandName.end(), command.end() - commandName.size()))
                if (m_command.back() != 'G')
                {
                    com.second();
                    m_command.clear();
                    return NORMAL;
                }

        }
        if (start_input_commands.count(command) != 0) {
            start_input_commands[command]();
            m_command.clear();
            return INPUT;
        }
        if (m_command.front() == 'r' && m_command.size() >= 2) {
            c->m_model->replace(c->cursor_row, c->cursor_col, 1, &m_command[1]);
            m_command.clear();
            return NORMAL;
        }
        if (m_command.back() == 'G' && m_command.size() > 1) {
            m_command.pop_back();
            if (m_command.find_first_not_of("0123456789") == std::string::npos)
            {
                c->cursor_row = std::stoi(m_command) - 1;
                m_command.clear();

            }

            return NORMAL;
        }
    }
    return NORMAL;
}

State Controller::NormalMode::wide_handle(std::string) {
    assert(false);
    return NORMAL;
}
