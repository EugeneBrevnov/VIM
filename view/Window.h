
#ifndef VIM_WINDOW_H
#define VIM_WINDOW_H

#include <curses.h>
#include <map>
#include "TextView.h"
#include "TextField.h"
#include "../controller/Controller.h"



class Window {
public:
    Window();

    void changeState(State newState);

    void setCursosPos(size_t row, size_t column);

    void modelReplaced(Model* pModel);

    void run(Controller* controller, Model* model);
private:
    TextField m_stateTextField;
    TextField m_commandTextField;
    TextView m_textView;
    State m_state;
    Controller *m_controller{};
    bool m_finished = false;
    Model *m_model{};
};


#endif //VIM_WINDOW_H
