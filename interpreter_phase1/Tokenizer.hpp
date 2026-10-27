#ifndef ALEXICALANALYZER_TOKENIZER_HPP
#define ALEXICALANALYZER_TOKENIZER_HPP

#include <cstddef>
#include <fstream>
#include <iosfwd>
#include <string>
#include <vector>

#include "Token.hpp"

class Tokenizer {
public:
    explicit Tokenizer(std::ifstream &inputStream);

    Token getToken();
    void ungetToken();
    void printProcessedTokens(std::ostream &output) const;

private:
    Token lastToken{};
    bool ungottenToken{false};
    std::ifstream &inputStream;
    std::vector<Token> tokens{};
    std::vector<int> indentStack{0};
    int dedentCount{0};
    std::size_t lineNumber{1};
    std::size_t columnNumber{1};
    bool lineContainsToken{false};

    bool getCharacter(char &character);
    std::string readIdentifier(char firstCharacter);
    std::int64_t readInteger(std::vector<char> number);
    double readDouble(std::vector<char> number);
    std::string readStringLiteral (char firstCharacter);

    static bool isDigit(char character);
    static bool isIdentifierStart(char character);
    static bool isIdentifierPart(char character);
    static bool isDiscardedWhitespace(char character);
};

#endif // ALEXICALANALYZER_TOKENIZER_HPP
