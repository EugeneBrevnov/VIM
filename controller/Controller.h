
#ifndef VIM_CONTROLLER_H
#define VIM_CONTROLLER_H

#include <deque>
#include <functional>
#include <cstring>
#include <algorithm>
#include "Signal.h"
#include "../view/TextView.h"
#include <string>
#include <curses.h>
#include <cassert>
#include <memory>
#include "../log.h"


enum State {
    NORMAL = 0,
    INPUT = 1,
    HELP = 2,
    COMMAND = 3,
    SEARCH = 4,
    EXIT
};

class Controller {
public:
    int64_t cursor_row, cursor_col;

    explicit Controller(Model *model) {
        m_model = model;
        m_textExists = false;
        m_searchDirectionToBottom = false;
        m_modes.resize(5);
        m_modes[NORMAL] = new NormalMode(this);
        m_modes[INPUT] = new InputMode(this);
        m_modes[HELP] = new HelpMode(this); //nullptr;
        m_modes[COMMAND] = new CommandMode(this);
        m_modes[SEARCH] = new SearchMode(this);
        cursor_col = cursor_row = 0;
        m_mode = nullptr;
        m_state = NORMAL;
        changeState(NORMAL);
    }

    void changeState(State state);

    void input(int c);

    void wide_input(std::string c);

    void replaceModel(Model* newModel);

SIGNAL(cursorPosChanged, size_t, size_t)
SIGNAL(stateChanged, State)
SIGNAL(modelReplaced, Model*)

private:
    class Mode {
    public:
        explicit Mode(Controller *con) : c(con) {}

        virtual void init() {};
        virtual void deinit() {};

        virtual State handle(int c) = 0;
        virtual State wide_handle(std::string c) = 0;

        virtual ~Mode() = default;

    protected:
        Controller *c;
    };

    class NormalMode : public Mode {
    public:
        explicit NormalMode(Controller *con) : Mode(con) {}
        State handle(int c) override;

        State wide_handle(std::string c) override;

    private:
        std::string m_command;
    };

    class CommandMode : public Mode {
    public:
        explicit CommandMode(Controller *con) : Mode(con) {}
        State handle(int code) override;

        State wide_handle(std::string c) override;

    private:
        //static std::string getCommandName(std::string const &s);

        //static std::string getCommandArgument(std::string const &s);
    };

    class InputMode : public Mode {
    public:
        explicit InputMode(Controller *con) : Mode(con) {}
        State handle(int s) override;

        State wide_handle(std::string c) override;
    };

    class SearchMode : public Mode {
    public:
        explicit SearchMode(Controller *con) : Mode(con) {}
        State handle(int s) override;

        State wide_handle(std::string c) override;
    };

    class HelpMode : public Mode {
    public:
        explicit HelpMode(Controller *con) : Mode(con), m_originalModel{} {}

        void init() override;
        void deinit() override;

        State handle(int s) override;

        State wide_handle(std::string c) override;

    private:
        Model* m_originalModel;
        std::unique_ptr<Model> m_helpModel;
    };

    void move_delta(int64_t delta_rows, int64_t delta_cols);

    void move_to(int64_t row, int64_t col, bool wide = false);

    void clamp_move();

    void clamp_row_col(bool wide = false);
   
    Model *m_model;
    Mode* m_mode;
    std::vector<Mode*> m_modes;
    std::string m_searchString;
    bool m_searchDirectionToBottom;
    bool m_textExists;
    std::string m_current_filename;
    State m_state;
    MyString m_copypaste;
};


#endif //VIM_CONTROLLER_H