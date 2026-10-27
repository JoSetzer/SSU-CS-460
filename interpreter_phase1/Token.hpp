#ifndef ALEXICALANALYZER_TOKEN_HPP
#define ALEXICALANALYZER_TOKEN_HPP

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <utility>

enum class Keyword {
    none,
    forKeyword,
    inKeyword,
    rangeKeyword,
    strKeyword,
    printKeyword
};

class Token {
public:
    [[nodiscard]] bool isEof() const { return _eof; }
    [[nodiscard]] bool isNewline() const { return _newline; }
    void markAsEof() { _eof = true; }
    void markAsNewline() { _newline = true; }
    [[nodiscard]] bool isIndent() const {return _isIndent;}
    [[nodiscard]] bool isDedent() const {return _isDedent;}
    void markAsIndent() { _isIndent = true; }
    void markAsDedent() { _isDedent = true; }

    void setLocation(std::size_t line, std::size_t column) {
        _lineNumber = line;
        _columnNumber = column;
    }
    [[nodiscard]] std::size_t lineNumber() const { return _lineNumber; }
    [[nodiscard]] std::size_t columnNumber() const { return _columnNumber; }

    void setSymbol(char symbol) { _symbol[0] = symbol; }
    void setSymbol(char firstSymbol, char secondSymbol) { _symbol[0] = firstSymbol; _symbol[1] = secondSymbol;}
    [[nodiscard]] const char *symbol() const { return _symbol; }

    [[nodiscard]] bool isOpenParen() const { return _symbol[0] == '('; }
    [[nodiscard]] bool isCloseParen() const { return _symbol[0] == ')'; }
    [[nodiscard]] bool isOpenBrace() const { return _symbol[0] == '{'; }
    [[nodiscard]] bool isCloseBrace() const { return _symbol[0] == '}'; }
    [[nodiscard]] bool isSemicolon() const { return _symbol[0] == ';'; }
    [[nodiscard]] bool isAssignmentOperator() const { return (_symbol[0] == '=')&&(_symbol[1] != '='); }
    [[nodiscard]] bool isCompareOperator() const { return (_symbol[0] == '=')&&(_symbol[1] == '='); }
    [[nodiscard]] bool isMultiplicationOperator() const { return _symbol[0] == '*'; }
    [[nodiscard]] bool isAdditionOperator() const { return _symbol[0] == '+'; }
    [[nodiscard]] bool isSubtractionOperator() const { return _symbol[0] == '-'; }
    [[nodiscard]] bool isModuloOperator() const { return _symbol[0] == '%'; }
    [[nodiscard]] bool isDivisionOperator() const { return (_symbol[0] == '/')&&(_symbol[1] != '/'); }
    [[nodiscard]] bool isFloorDivisionOperator() const { return (_symbol[0] == '/')&&(_symbol[1] == '/'); }
    [[nodiscard]] bool isGreaterThanOperator() const { return (_symbol[0] == '>')&&(_symbol[1] != '='); }
    [[nodiscard]] bool isGreaterThanOrEqualOperator() const { return (_symbol[0] == '>')&&(_symbol[1] == '='); }
    [[nodiscard]] bool isLessThanOperator() const { return (_symbol[0] == '<')&&(_symbol[1] != '='); }
    [[nodiscard]] bool isLessThanOrEqualOperator() const { return (_symbol[0] == '<')&&(_symbol[1] == '='); }
    [[nodiscard]] bool isNotEqualOperator() const { return (_symbol[0] == '!')&&(_symbol[1] == '='); }
    [[nodiscard]] bool isColon() const { return _symbol[0] == ':'; }
    [[nodiscard]] bool isComma() const { return _symbol[0] == ','; }


    void setIdentifier(std::string identifier) { _identifier = std::move(identifier); }
    [[nodiscard]] bool isIdentifier() const { return !_identifier.empty(); }
    [[nodiscard]] const std::string &identifier() const { return _identifier; }

    void setKeyword(Keyword keyword) { _keyword = keyword; }
    [[nodiscard]] bool isKeyword() const { return _keyword != Keyword::none; }
    [[nodiscard]] bool isForKeyword() const { return _keyword == Keyword::forKeyword; }
    [[nodiscard]] bool isInKeyword() const { return _keyword == Keyword::inKeyword; }
    [[nodiscard]] bool isRangeKeyword() const { return _keyword == Keyword::rangeKeyword; }
    [[nodiscard]] bool isStrKeyword() const { return _keyword == Keyword::strKeyword; }
    [[nodiscard]] bool isPrintKeyword() const { return _keyword == Keyword::printKeyword; }
    [[nodiscard]] Keyword keyword() const { return _keyword; }

    void setIntegerValue(std::int64_t value) {
        _integerValue = value;
        _isInteger = true;
    }
    void setDoubleValue(double value) {
        _doubleValue = value;
        _isDouble = true;
    }
    void setStringValue(std::string value) {
        _stringValue = value;
        _isString = true;
    }
    void setBooleanValue(bool value) {
        _booleanValue = value;
        _isBoolean = true;
    }
    [[nodiscard]] bool isInteger() const { return _isInteger; }
    [[nodiscard]] bool isDouble() const { return _isDouble; }
    [[nodiscard]] bool isString() const { return _isString; }
    [[nodiscard]] bool isBoolean() const { return _isBoolean; }
    [[nodiscard]] std::int64_t integerValue() const { return _integerValue; }
    [[nodiscard]] double doubleValue() const { return _doubleValue; }
    [[nodiscard]] std::string stringValue() const { return _stringValue; }
    [[nodiscard]] bool booleanValue() const { return _booleanValue; }

    void print(std::ostream &output) const;

private:
    std::string _identifier{};
    Keyword _keyword{Keyword::none};
    bool _eof{false};
    bool _newline{false};
    bool _isInteger{false};
    bool _isDouble{false};
    bool _isString{false};
    bool _isBoolean{false};
    bool _isIndent{false};
    bool _isDedent{false};
    char _symbol[2]{'\0','\0'};
    std::int64_t _integerValue{0};
    double _doubleValue{0};
    std::string _stringValue{};
    bool _booleanValue{false};
    std::size_t _lineNumber{0};
    std::size_t _columnNumber{0};
};

#endif // ALEXICALANALYZER_TOKEN_HPP
