
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cassert>

class MyString {

private:

    char* str; //pointer to the contents of the string

    int str_len; // the current length of the string

    int current_capacity = 64; // the maximum capacity of the line

    void check_capacity(int new_len);

    void change_capacity(int new_len);

public:

    bool insert(int index, const char* string, int count);

    bool insert(int index, int count, char symbol);

    bool erase(int index, int count);

    bool insert(int index, const char* string);

    bool insert(int index,const std::string& string);

    bool insert(int index,const std::string& string, int count);

    bool append(int count, char symbol);

    bool append(const MyString& string);

    bool append(std::string& string);

    bool append(std::string& string, int index, int count);

    bool append(const char* string);

    bool append(const char* string, int index, int count);

    bool replace(int index, int count, const char* string);

    bool replace(int index, int count, const std::string& string);

    MyString substr(int index) const;

    MyString substr(int index, int count) const;

    int find(const char* string, int index) const;

    int find(const char* string) const;

    int find(std::string& string) const;

    int find(std::string& string, int index) const;

    MyString();

    MyString(const MyString& new_string);

    MyString(const char* new_string);

    MyString(const std::initializer_list<char>& list);

    MyString(std::string& new_string);

    MyString(const char* new_string, int new_string_len);

    MyString(int count, char symbol);

    ~MyString();

    void reset();

    MyString operator+(const MyString& string);

    MyString operator+(const char* string);

    MyString operator+(std::string& string);

    MyString& operator+=(const char* string);

    MyString& operator+=(std::string& string);

    MyString& operator+=(MyString string);

    bool operator > (const MyString& string)const;

    bool operator < (const MyString& string) const;

    bool operator >= (const MyString& string) const;

    bool operator <= (const MyString& string) const;

    bool operator != (const MyString& string) const;

    bool operator==(const MyString& string) const;

    MyString& operator=(const char* string);

    MyString& operator=(std::string& string);

    MyString& operator=(char symbol);

    MyString& operator=(MyString& string);

    char& operator [](int idx);

    const char* c_str() const;

    int size();

    int length();

    bool empty();

    int capacity();

    void clear();

    void shrink_to_fit();

    friend std::ostream& operator<<(std::ostream& out, const MyString& string);

    friend std::istream& operator>>(std::istream& in, MyString string);

};
