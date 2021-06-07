
#ifndef VIM_LOG_H
#define VIM_LOG_H


class Log {
public:
    static Log* get() {
        static Log instance;
        return &instance;
    }

    template <typename T>
    void log(T const& s) {
        m_out << std::to_string(s) << std::endl;
    }

    void log(char s) {
        m_out << s << std::endl;
    }

    void log(std::string const& s) {
        m_out << s << std::endl;
    }

private:
    Log() : m_out("log.txt") {}

    std::ofstream m_out;
};

#endif //VIM_LOG_H
