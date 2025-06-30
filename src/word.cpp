
#include <cassert>

// for optional type strictness
class Word
//{
{
private:
    string word;
public:
    Word(string w)
    {
        word = "";
        // just remove bad characters for now
        for(int i = 0 ; i < int(w.length()) ; i++)
            if((w[i] != ' ') and
                    (w[i] != '\t') and
                    (w[i] != ',') and
                    (w[i] != '\n'))
            {
                word += w[i];
            }

    }
    Word(char w)
    {
        word = "";
        word += w;
    }

    int length() const
    {
        return word.length();
    }


    Word inverse()
    {
        Word w = word;
        std::string rev;
        for(int i = 0 ; i < w.length() ; i++)
        {
            char c = w[i];
            rev += islower(c) ? toupper(c) : tolower(c);
        }
        return Word(rev);
    }

    // For ordered containers like std::set
    bool operator<(const Word& other) const
    {
        return word < other.word;
    }
    // For ordered containers like std::set
    bool operator>(const Word& other) const
    {
        return word > other.word;
    }

//bool operator==(const Word& lhs, const Word& rhs) {
//   return lhs.str() == rhs.str();
//}

    bool operator==(const Word& other) const
    {
        return word == other.word;
    }
    bool operator!=(const Word& other) const
    {
        return word != other.word;
    }

    // Word + Word
    Word operator+(const Word& other) const
    {
        return Word(word + other.word);
    }

    // Word + std::string
    Word operator+(const std::string& other) const
    {
        return Word(word + other);
    }

    // std::string + Word
    friend Word operator+(const std::string& lhs,
                          const Word& rhs)
    {
        return Word(lhs + rhs.word);
    }

    //TODO: figure out how to test this
    // allow implicit conversion to std::string
    operator std::string() const
    {
        return word;
    }

    // allow explicit conversion to std::string
    string str() const
    {
        return word;
    }

    // const access: safe for read-only contexts
    char operator[](size_t i) const
    {
        return word[i];
    }

    // non-const access: allows mutation
    char& operator[](size_t i)
    {
        return word[i];
    }

    Word& operator+=(char c)
    {
        word += c;
        return *this;
    }
    Word& operator+=(string c)
    {
        word += c;
        return *this;
    }

    Word& operator+=(const Word& other)
    {
        word += other.word;
        return *this;
    }

    size_t find(const Word& pattern) const
    {
        return word.find(pattern.word);
    }
    size_t find(const string& pattern) const
    {
        return word.find(pattern);
    }

    Word& erase(size_t pos, size_t len = std::string::npos)
    {
        word.erase(pos, len);
        return *this;
    }

// Replace a substring at position `pos`, of length `len`, with another Word
    Word& replace(size_t pos, size_t len,
                  const Word& replacement)
    {
        word.replace(pos, len, replacement.word);
        return *this;
    }
    Word& replace(size_t pos, size_t len,
                  const string& replacement)
    {
        word.replace(pos, len, replacement);
        return *this;
    }

    // TODO: make a replaceAll(string pattern);

    string substr(size_t pos,
                  size_t len = std::string::npos) const
    {
        return word.substr(pos, len);
    }
    Word subword(size_t pos, size_t len = std::string::npos) const
    {
        return Word(word.substr(pos, len));
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const Word& obj);
};
std::ostream& operator<<(std::ostream& os, const Word& obj)
{
    os << obj.word;
    return os;
}

// Presentation < Generator : Relation >
class Relation : public Word
{
public:
    // type conversion or initializer
    explicit Relation(const std::string& w) : Word(w) {};
};

class Generator: public Word
{
public:

    explicit Generator(char w) : Word(std::string(1, w)) {}    

    explicit Generator(const std::string& w)
        : Word(w) 
    {

        if (w.size() != 1)
            throw std::invalid_argument("Generator must be a single character string.");

    }
};
//}
