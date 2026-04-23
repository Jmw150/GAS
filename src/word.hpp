#pragma once
#include <string>
#include <iostream>
#include <stdexcept>

using std::string;

class Word
{
private:
    string word;
public:
    Word(string w)
    {
        word = "";
        for(size_t i = 0; i < w.length(); i++)
            if(w[i] != ' ' &&
               w[i] != '\t' &&
               w[i] != ',' &&
               w[i] != '\n')
                word += w[i];
    }
    Word(char w)
    {
        word = "";
        word += w;
    }

    size_t length() const
    {
        return word.length();
    }

    Word inverse() const
    {
        std::string result;
        for(int i = (int)word.length() - 1; i >= 0; i--)
        {
            char c = word[i];
            result += islower(c) ? toupper(c) : tolower(c);
        }
        return Word(result);
    }

    Word& insert(std::size_t pos, const Word& other)
    {
        if(pos > word.size())
            pos = word.size();
        word.insert(pos, other.word);
        return *this;
    }

    bool operator<(const Word& other) const { return word < other.word; }
    bool operator>(const Word& other) const { return word > other.word; }
    bool operator==(const Word& other) const { return word == other.word; }
    bool operator!=(const Word& other) const { return word != other.word; }

    Word operator+(const Word& other) const { return Word(word + other.word); }
    Word operator+(const std::string& other) const { return Word(word + other); }
    friend Word operator+(const std::string& lhs, const Word& rhs)
    {
        return Word(lhs + rhs.word);
    }

    operator std::string() const { return word; }
    string str() const { return word; }

    char operator[](size_t i) const { return word[i]; }
    char& operator[](size_t i) { return word[i]; }

    Word& operator+=(char c) { word += c; return *this; }
    Word& operator+=(string c) { word += c; return *this; }
    Word& operator+=(const Word& other) { word += other.word; return *this; }

    size_t find(const Word& pattern) const { return word.find(pattern.word); }
    size_t find(const string& pattern) const { return word.find(pattern); }

    Word& erase(size_t pos, size_t len = std::string::npos)
    {
        word.erase(pos, len);
        return *this;
    }

    Word& replace(size_t pos, size_t len, const Word& replacement)
    {
        word.replace(pos, len, replacement.word);
        return *this;
    }
    Word& replace(size_t pos, size_t len, const string& replacement)
    {
        word.replace(pos, len, replacement);
        return *this;
    }

    string substr(size_t pos, size_t len = std::string::npos) const
    {
        return word.substr(pos, len);
    }
    Word subword(size_t pos, size_t len = std::string::npos) const
    {
        return Word(word.substr(pos, len));
    }

    friend std::ostream& operator<<(std::ostream& os, const Word& obj);
};

inline std::ostream& operator<<(std::ostream& os, const Word& obj)
{
    os << obj.word;
    return os;
}

class Relation : public Word
{
public:
    explicit Relation(const std::string& w) : Word(w) {}
};

class Generator : public Word
{
public:
    explicit Generator(char w) : Word(std::string(1, w)) {}

    explicit Generator(const std::string& w) : Word(w)
    {
        if(w.size() != 1)
            throw std::invalid_argument("Generator must be a single character string.");
    }
};
