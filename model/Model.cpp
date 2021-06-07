
#include "Model.h"

void Model::open(const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw std::runtime_error("can't open file " + filename);
    read(in);
    m_filename = filename;
}


void Model::read(std::istream &in) {
    m_lines.clear();
    std::string line;
    while (std::getline(in, line)) {

        m_lines.push_back(MyString(line));
    }
    m_modified = false;
    if (m_lines.empty()) m_lines.emplace_back("");
    m_filename = "";
}



void Model::write(const std::string &l_filename) {
    std::string filename = l_filename.empty() ? m_filename : l_filename;
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("can't open file " + filename);
    for (auto& line : m_lines) {
        out << line << std::endl;
    }
    m_modified = false;
}

void Model::newLine(int i) {
    MyString str;
    m_lines.insert(m_lines.begin() + 1 + i, str);
    m_lines[i + 1].shrink_to_fit();
}


MyString& Model::line(size_t line) {
    return m_lines[line];
}

size_t Model::n_lines() const {
    return m_lines.size();
}

size_t Model::lineSize(size_t i) {
    return m_lines[i].size();
}

void Model::removeLine(size_t i) {

    m_modified = true;
    m_lines.erase(m_lines.cbegin() + i);
}

void Model::insert(size_t line, size_t pos, MyString str) {
    m_modified = true;
    m_lines[line].insert(pos, str.c_str());
}

void Model::push_back() {
    m_lines.push_back("\0");

}

void Model::erase(size_t line, size_t pos, size_t len) {
    m_modified = true;
    m_lines[line].erase(pos, len);
}

void Model::replace(size_t line, size_t pos, size_t len, char* const& str) {
    m_modified = true;
    m_lines[line].replace(pos, len, str);
}

bool Model::modified() {
    return m_modified;
}