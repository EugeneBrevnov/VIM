#include "Controller.h"

State Controller::SearchMode::handle(int s) {
    return SEARCH;
}


State Controller::SearchMode::wide_handle(std::string s) {
    c->m_searchDirectionToBottom = s.front() == '/';
    c->m_searchString = s.substr(1);
    return NORMAL;
}
