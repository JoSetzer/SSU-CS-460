#include <cctype>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#include "Tokenizer.hpp"

bool Tokenizer::isDigit(char character) {
    return std::isdigit(static_cast<unsigned char>(character)) != 0;
}

bool Tokenizer::isIdentifierStart(char character) {
    return std::isalpha(static_cast<unsigned char>(character)) != 0 || character == '_';
}

bool Tokenizer::isIdentifierPart(char character) {
    return std::isalnum(static_cast<unsigned char>(character)) != 0 || character == '_';
}

bool Tokenizer::isDiscardedWhitespace(char character) {
    return character != '\n' && std::isspace(static_cast<unsigned char>(character)) != 0;
}

bool Tokenizer::getCharacter(char &character) {
    if (!inputStream.get(character))
        return false;

    if (character == '\n') {
        ++lineNumber;
        columnNumber = 1;
    } else {
        ++columnNumber;
    }
    return true;
}

std::string Tokenizer::readStringLiteral(char firstCharacter) {
   	std::string stringLiteral = "";
       	bool terminated = false;	
	while (inputStream.peek() != std::char_traits<char>::eof()) {
        	char character = static_cast<char>(inputStream.peek());
        	if (character == firstCharacter) {
			getCharacter(character);
			terminated = true;
			break;
		}
		if (character == '\\' ||
	    	    character == '\n' ||
	    	    character == '\t' ||
	    	    !std::isprint(static_cast<unsigned char>(character))) {
            		std::cerr << "Invalid char in string " << lineNumber
                		  << ", column " << columnNumber << ".\n";
            		std::exit(EXIT_FAILURE);
		
		}
        	getCharacter(character);
        	stringLiteral += character;
    	}
	if (!terminated) {
            	std::cerr << "Missing closing quotation for string " << lineNumber
                	  << ", column " << columnNumber << ".\n";
        	std::exit(EXIT_FAILURE);
	}
    	return stringLiteral;
}

std::string Tokenizer::readIdentifier(char firstCharacter) {
    std::string identifier{firstCharacter};
    while (inputStream.peek() != std::char_traits<char>::eof()) {
        char character = static_cast<char>(inputStream.peek());
        if (!isIdentifierPart(character))
            break;
        getCharacter(character);
        identifier += character;
    }
    return identifier;
}

double Tokenizer::readDouble(std::vector<char> number) {
	if (number.size() < 1) {
            		std::cerr << "Double literal is too small at line " << lineNumber
                      		  << ", column " << columnNumber << ".\n";
            		std::exit(EXIT_FAILURE);
	}
	if (number[0] == '.') {
            		std::cerr << "Double literal leading decimal point " << lineNumber
                      		  << ", column " << columnNumber << ".\n";
            		std::exit(EXIT_FAILURE);
	}
	if (number[number.size()-1] == '.') {
            		std::cerr << "Double literal trailing decimal point " << lineNumber
                      		  << ", column " << columnNumber << ".\n";
            		std::exit(EXIT_FAILURE);
	}
	std::string doubleString(number.begin(), number.end());
	double value = std::stod(doubleString);
    	return value;
}

std::int64_t Tokenizer::readInteger(std::vector<char> number) {
	if (number.size() < 1) {
            		std::cerr << "Integer literal is too small at line " << lineNumber
                      		  << ", column " << columnNumber << ".\n";
            		std::exit(EXIT_FAILURE);

	}
	std::int64_t value = number[0] - '0';
	for (std::size_t i = 1; i < number.size(); i++){
        	const int digit = number[i] - '0';
        	if (value > (std::numeric_limits<std::int64_t>::max() - digit) / 10) {
            		std::cerr << "Integer literal is too large at line " << lineNumber
                      		  << ", column " << columnNumber << ".\n";
            		std::exit(EXIT_FAILURE);
        	}
        	value = value * 10 + digit;
    	}
    return value;
}

Tokenizer::Tokenizer(std::ifstream &stream) : inputStream{stream} {}

Token Tokenizer::getToken() {
    if (ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    if (dedentCount > 0) {
        dedentCount--;
        Token token;
        token.setLocation(lineNumber, columnNumber);
        token.markAsDedent();
        tokens.push_back(token);
        return lastToken = token;
    }

    while (inputStream.peek() != std::char_traits<char>::eof()) {
        char character = static_cast<char>(inputStream.peek());

        if (character == '\t' && !lineContainsToken) {
            std::cerr << "Error: Tab is not allowed.\n";
            std::exit(EXIT_FAILURE);
        }

        if (isDiscardedWhitespace(character)) {
            getCharacter(character);
            continue;
        }

        if (character == '\n') {
            const auto newlineLine = lineNumber;
            const auto newlineColumn = columnNumber;
            getCharacter(character);

            if (lineContainsToken) {
                Token token;
                token.setLocation(newlineLine, newlineColumn);
                token.markAsNewline();
                lineContainsToken = false;
                tokens.push_back(token);
                return lastToken = token;
            }

            // Newlines on blank or whitespace-only lines are insignificant.
            continue;
        }

        if (!lineContainsToken) {
            int indent = columnNumber - 1;
            Token token;
            token.setLocation(lineNumber, columnNumber);

            if (indent > indentStack.back()) {
                indentStack.push_back(indent);
                lineContainsToken = true;

                token.markAsIndent();
                tokens.push_back(token);
                return lastToken = token;

            }
            else if (indent < indentStack.back()) {
                while (indentStack.back() > indent) {
                    indentStack.pop_back();
                    dedentCount++;
                }
                if (indentStack.back() != indent) {
                    std::cerr << "Error: DEDENT at line " << token.lineNumber()
                  << ", column " << token.columnNumber() << ".\n";
                    std::exit(EXIT_FAILURE);
                }
                lineContainsToken = true;
                dedentCount--;
                token.markAsDedent();
                tokens.push_back(token);
                return lastToken = token;
            }
        }

        break;
    }

    Token token;
    token.setLocation(lineNumber, columnNumber);

    if (inputStream.peek() == std::char_traits<char>::eof()) {
        if (inputStream.bad()) {
            std::cerr << "Error while reading the input stream in Tokenizer.\n";
            std::exit(EXIT_FAILURE);
        }
        if (lineContainsToken) {
            token.markAsNewline();
            tokens.push_back(token);
            lineContainsToken = false;
            return lastToken = token;
        }
        if (indentStack.back() > 0) {
            while (indentStack.back() > 0) {
                indentStack.pop_back();
                dedentCount++;
            }
            token.markAsDedent();
            tokens.push_back(token);
            dedentCount--;
            return lastToken = token;
        }

        token.markAsEof();
    } else {
        char character, peekChar;
        getCharacter(character);

        if (isDigit(character)) {
		bool isDouble = false;
		std::vector<char> number;
		number.push_back(character);

		while (inputStream.peek() != std::char_traits<char>::eof()) {
			char next = static_cast<char>(inputStream.peek());	
			
			if (isDigit(next)) {
				getCharacter(next);
				number.push_back(next);
			} else if (next == '.'){
				if (isDouble == true) {
					std::cerr << "Error: Too many decimals in double " << token.lineNumber()
				  		  << ", column " << token.columnNumber() << ".\n";
					std::exit(EXIT_FAILURE);
				}
				isDouble = true;
				getCharacter(next);
				//if (!isDigit(static_cast<char>(inputStream.peek()))) {
				//	std::cerr << "Error: No value after decimal point in double " << token.lineNumber()
				//  		  << ", column " << token.columnNumber() << ".\n";
				//	std::exit(EXIT_FAILURE);
				//}
				number.push_back(next);
			} else
				break;
		}
		if (isDouble)
			token.setDoubleValue(readDouble(number));
		else	
            		token.setIntegerValue(readInteger(number));
        } else if (character == ';' || character == '+' || character == '-' ||
                   character == '*' || character == ',' || character == '%' ||
                   character == ')' || character == '(' || character == '{' ||
                   character == '}' || character == ':') {
            token.setSymbol(character);
        } else if (character == '=' || character == '>' || character == '<') {
		//peekChar = static_cast<char>(inputStream.peek());
		if (static_cast<char>(inputStream.peek()) == '=') {
			getCharacter(peekChar);
			token.setSymbol(character, peekChar);
		} else {
			token.setSymbol(character);
		}
	} else if (character == '!'){
		//peekChar = static_cast<char>(inputStream.peek());
		if (static_cast<char>(inputStream.peek()) == '=') {
			getCharacter(peekChar);
			token.setSymbol(character, peekChar);
		} else {
			std::cerr << "Error: lone bang symbol at line " << token.lineNumber()
				  << ", column " << token.columnNumber() << ".\n";
			std::exit(EXIT_FAILURE);
		}
	} else if (character == '/'){
		if (static_cast<char>(inputStream.peek()) == '/') {
			getCharacter(peekChar);
			token.setSymbol(character, peekChar);
		} else {
			token.setSymbol(character);
		}
	} else if (character == '\'' || character == '\"') {
		std::string stringLiteral = readStringLiteral(character);
		token.setStringValue(stringLiteral);
	} else if (isIdentifierStart(character)) {
            	std::string identifier = readIdentifier(character);
            if (identifier == "for")
                token.setKeyword(Keyword::forKeyword);
            else if (identifier == "print")
                token.setKeyword(Keyword::printKeyword);
            else if (identifier == "in")
                token.setKeyword(Keyword::inKeyword);
            else if (identifier == "range")
                token.setKeyword(Keyword::rangeKeyword);
            else if (identifier == "str")
                token.setKeyword(Keyword::strKeyword);
            else if (identifier == "True")
                token.setBooleanValue(true);
            else if (identifier == "False")
                token.setBooleanValue(false);
            else
                token.setIdentifier(std::move(identifier));
        } else {
            std::cerr << "Unknown character in input at line " << token.lineNumber()
                      << ", column " << token.columnNumber() << ": '"
                      << character << "'.\n";
            std::exit(EXIT_FAILURE);
        }

        lineContainsToken = true;
    }

    tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    if (ungottenToken) {
        std::cerr << "Tokenizer supports only one ungotten token.\n";
        std::exit(EXIT_FAILURE);
    }
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens(std::ostream &output) const {
    for (const auto &token : tokens) {
        token.print(output);
        output << '\n';
    }
}
