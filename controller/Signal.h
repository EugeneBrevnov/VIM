
#ifndef VIM_SIGNAL_H
#define VIM_SIGNAL_H


#include <vector>
#include <functional>
#include <iostream>

#define SIGNAL(name, ...) \
public: \
    void addHandler_ ## name(Signal<__VA_ARGS__>::HandlerType handler) { \
        m_signal_ ## name.addHandler(std::move(handler)); \
    } \
private: \
    Signal<__VA_ARGS__> m_signal_ ## name;


template <typename... Args>
class Signal {
public:
    using HandlerType = std::function<void(Args...)>;

    void addHandler(HandlerType func) {
        m_handlers.push_back(std::move(func));
    }

    void emit(Args... args) {
        for (auto && handler : m_handlers) {
            try {
                handler(args...);
            } catch (...) {
                std::cerr << "Exception from signal" << std::endl;
            }
        }
    }

private:
    std::vector<HandlerType> m_handlers;
};


#endif //VIM_SIGNAL_H
