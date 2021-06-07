#ifndef VIM_MODEL_H
#define VIM_MODEL_H

#include <vector>
#include <string>
#include <fstream>
#include "../MyString/MyString.h"

class Model {

public:
    void open(std::string const& filename);

    void read(std::istream &in);

    void write(std::string const& filename);

    MyString& line(size_t line);

    size_t n_lines() const;

    size_t lineSize(size_t i);

    void removeLine(size_t i);

    void insert(size_t line, size_t pos, MyString str);

    void push_back();

    void erase(size_t line, size_t pos, size_t len);

    void replace(size_t line, size_t pos, size_t len, char* const& str);

    bool modified();

    void newLine(int i);

    std::string m_filename;

private:
    bool m_modified = false;
   
    std::vector<MyString> m_lines;
  
};

#endif //VIM_MODEL_H
