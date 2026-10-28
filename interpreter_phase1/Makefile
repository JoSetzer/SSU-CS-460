CXX = c++
CXXFLAGS = -g -std=c++20 -Wall -Wextra -Wpedantic
TARGET = interpreter.x
SOURCES = ValueDescriptor.cpp Token.cpp Tokenizer.cpp Parser.cpp Expr.cpp SymbolTable.cpp Statements.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Each object depends on every header it includes, directly or indirectly.
ValueDescriptor.o: ValueDescriptor.cpp ValueDescriptor.h
Token.o: Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp Token.hpp
SymbolTable.o: SymbolTable.cpp SymbolTable.hpp ValueDescriptor.h
Expr.o: Expr.cpp Expr.hpp SymbolTable.hpp Token.hpp ValueDescriptor.h
Statements.o: Statements.cpp Statements.hpp Expr.hpp SymbolTable.hpp Token.hpp ValueDescriptor.h
Parser.o: Parser.cpp Parser.hpp Expr.hpp Statements.hpp SymbolTable.hpp Token.hpp Tokenizer.hpp ValueDescriptor.h
main.o: main.cpp Parser.hpp Expr.hpp Statements.hpp SymbolTable.hpp Token.hpp Tokenizer.hpp ValueDescriptor.h

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)
