#include "MyString.h"

void MyString::check_capacity(int new_len) {
    if (new_len + 1 >= current_capacity || str == nullptr) {
        change_capacity(new_len);
    }
}


void MyString::change_capacity(int new_len) {
    current_capacity = new_len + 1;
    char* new_str = new char[current_capacity];
    if (str != nullptr) {
        std::copy(str, str + str_len + 1, new_str);
    }
    for (int i = str_len; i < current_capacity; i++)
        new_str[i] = '\0';
    str = new_str;
}

bool MyString::insert(int index, const char* string, int count) {
    if (string == NULL || index < 0 || count<1 || index > str_len)
        return false;
    check_capacity(str_len + count);
    std::copy(str + index, str + str_len, str + index + count);
    std::copy(string, string + count, str + index);
    str_len += count;
    return true;
}

bool MyString::insert(int index, int count, char symbol) {
    if (index < 0 || count<1 || index > str_len)
        return false;
    char* string = new char[count];
    for (int i = 0; i < count; i++)
        string[i] = symbol;
    bool success = insert(index, string, count);
    delete[] string;
    return success;
}

bool MyString::erase(int index, int count) {
    if (index >= 0 && count > 0)
    {
        std::copy(str + index + count, str + str_len + 1, str + index);
        str_len -= count;
        shrink_to_fit();
        return true;
    }
    return false;
}

bool MyString::insert(int index, const char* string) {
    return insert(index, string, strlen(string));
}

bool MyString::insert(int index,const std::string& string) {
    return insert(index, string.c_str(), string.size());
}

bool MyString::insert(int index, const std::string& string, int count) {
    return  insert(index, string.c_str(), count);
}

bool MyString::append(int count, char symbol) {
    check_capacity(str_len + count);
    for (int y = 0; y < count; y++)
    {
        str[str_len + y] = symbol;
    }
    str_len += count;
    str[str_len] = '\0';
    return true;
}

bool MyString::append(const MyString& string) {
    return append(string.str, 0, string.str_len);
}

bool MyString::append(std::string& string) {
    return  append(string, 0, string.size());
}

bool MyString::append(std::string& string, int index, int count) {
    if (index < 0 || count < 1)
        return false;
    char* cstr = new char[string.length() + 1];
    strcpy(cstr, string.c_str());
    bool result = append(cstr, index, string.size());
    delete[] cstr;
    return result;
}

bool MyString::append(const char* string) {
    return append(string, 0, strlen(string));
}

bool MyString::append(const char* string, int index, int count) {
    if (string == NULL || index < 0 || count < 1 || index + count > strlen(string))
        return false;
    check_capacity(str_len + count);
    std::copy(string + index, string + count, str + str_len);
    str_len += count;
    str[str_len] = '\0';
    return true;
}

bool MyString::replace(int index, int count, const char* string) {
    if (erase(index, count))
        return insert(index, string);
    else return false;
}

bool MyString::replace(int index, int count, const std::string& string) {
    if (erase(index, count))
        return insert(index, string);
    else return false;
}

MyString MyString::substr(int index) const {
    if (index < str_len && index >= 0)
        return MyString(str + index, str_len - index);
    else return MyString{ "" };
}

MyString MyString::substr(int index, int count) const {
    if (index < str_len && index >= 0 && count > 0 && index + count < str_len)
        return MyString(str + index, count);
    else return MyString{ "" };
}

int MyString::find(const char* string, int index) const {
    int idx = 0;
    int k = 0;
    for (int i = index; str[i] != '\0'; i++) {
        for (int j = i, k = 0; string[k] != '\0' && str[j] == string[k]; j++, k++) {
            if (k > 0 && k == strlen(string) - 1)
            {
                idx = i;
                break;
            }
        }
    }
    return idx;
}

int MyString::find(const char* string) const {
    return find(string, 0);
}

int MyString::find(std::string& string) const {
    return find(string.c_str());
}

int MyString::find(std::string& string, int index) const {
    return find(string.c_str(), index);
}
MyString::MyString() : MyString("\0") { }

MyString::MyString(const MyString& new_string) : MyString(new_string.str, new_string.str_len) {	}

MyString::MyString(const char* new_string) : MyString(new_string, strlen(new_string)) { }

MyString::MyString(const std::initializer_list<char>& list) : MyString(list.begin(), list.size()) { }

MyString::MyString(std::string& new_string) : MyString(new_string.c_str(), new_string.size()) {	}

MyString::MyString(const char* string, int string_len) {
    str = nullptr;
    str_len = string_len;
    check_capacity(str_len);
    std::copy(string, string + str_len, str);
    str[str_len] = '\0';
}

MyString::MyString(int count, char symbol) {
    check_capacity(count);
    for (int i = 0; i < count; i++)
    {
        *(str + i) = symbol;
    }
    str_len = count;
    str[count] = '\0';
}

MyString::~MyString()
{
    reset();
}

void MyString::reset() {
    if (str) delete[] str;
    str = NULL;
    str_len = 0;
    current_capacity = 0;
}

MyString MyString::operator+(const MyString& string) {
    MyString new_string(*this);
    new_string.append(string);
    return new_string;
}

MyString MyString::operator+(const char* string) {
    MyString new_string(*this);
    new_string.append(string);
    return new_string;
}

MyString MyString::operator+(std::string& string) {
    MyString new_string(*this);
    new_string.append(string);
    return new_string;
}

MyString& MyString::operator+=(const char* string) {
    append(string);
    return *this;
}

MyString& MyString::operator+=(std::string& string) {
    append(string);
    return *this;
}

MyString& MyString::operator+=(MyString string) {
    append(string.str);
    return *this;
}

bool MyString::operator > (const MyString& string) const {
    return strcmp(str, string.str);
}

bool MyString::operator < (const MyString& string) const {
    return strcmp(str, string.str);
}

bool MyString::operator >= (const MyString& string) const {
    return strcmp(str, string.str);
}

bool MyString::operator <= (const MyString& string) const {
    return strcmp(str, string.str);
}

bool MyString::operator != (const MyString& string) const {
    return !(*this == string);
}

bool MyString::operator==(const MyString& string) const {
    bool identical = true;
    if (str_len != string.str_len) {
        return false;
    }
    else {
        for (int i = 0; i < str_len; i++) {
            if (str[i] != string.str[i]) identical = false;
        }
    }
    return identical;
} 

MyString& MyString::operator=(MyString& string)
{
    check_capacity(strlen(string.c_str()));
    std::copy(string.c_str(), string.c_str() + strlen(string.c_str()), str);
    str_len = strlen(string.c_str());
    str[str_len] = '\0';
    return *this;
};

MyString& MyString::operator=(const char* string) {
    check_capacity(strlen(string));
    std::copy(string, string + strlen(string), str);
    str_len = strlen(string);
    str[str_len] = '\0';
    return *this;
}

MyString& MyString::operator=(std::string& string) {
    check_capacity(string.size());
    std::copy(string.c_str(), string.c_str() + string.size(), str);
    str_len = string.size();
    str[str_len] = '\0';
    return *this;
}

MyString& MyString::operator=(char symbol) {
    check_capacity(1);
    str[0] = symbol;
    str[1] = '\0';
    return *this;
}

char& MyString::operator [](int index) {
    if (index < str_len)
        return str[index];
    else
        return str[str_len];
}

const char* MyString::c_str() const {
    return str;
}

int MyString::size() {
    return str_len;
}

int MyString::length() {
    return str_len;
}

bool MyString::empty() {
    return size() == 0;
}

int MyString::capacity() {
    return this->current_capacity;
}

void MyString::clear() {
    reset();
}

void MyString::shrink_to_fit() {
    if (current_capacity != str_len + 1) {
        current_capacity = str_len + 1;
        char* new_str = new char[current_capacity];
        std::copy(str, str + current_capacity, new_str);
        delete[] str;
        str = new_str;
    }
}


std::istream& operator>>(std::istream& in, MyString string) {
    in.read(string.str, string.str_len);
    return in;
}

std::ostream& operator<<(std::ostream& out, const MyString& string) {
    out.write(string.str, string.str_len);
    return out;
}