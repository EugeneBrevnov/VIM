#include <iostream>
#include "view/Window.h"
#include <vector>


int main() {

    Window win;
    Model model;

    //model.open("Makefile");
    Controller c(&model);
    c.addHandler_cursorPosChanged([&](size_t row, size_t column) {
        win.setCursosPos(row, column);
    });
    c.addHandler_stateChanged([&](State st) {
        win.changeState(st);
    });
    c.addHandler_modelReplaced([&](Model* newModel) {
        win.modelReplaced(newModel);
    });
    win.run(&c, &model);
    return 0;
}
