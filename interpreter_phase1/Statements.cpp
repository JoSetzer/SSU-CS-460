#include <iostream>
#include <string>
#include <utility>

#include "Statements.hpp"

Statements::~Statements() {
    for (auto *statement : statements)
        delete statement;
}

void Statements::addStatement(Statement *statement) {
    statements.push_back(statement);
}

void Statements::print() const {
    for (const auto *statement : statements)
        statement->print();
}

void Statements::evaluate(SymbolTable &symbolTable) const {
    for (const auto *statement : statements)
        statement->evaluate(symbolTable);
}

AssignmentStatement::AssignmentStatement(
    std::string variableName,
    ExprNode *expression)
    : variableName{std::move(variableName)}, expression{expression} {}

AssignmentStatement::~AssignmentStatement() {
    delete expression;
}

void AssignmentStatement::evaluate(SymbolTable &symbolTable) const {
    symbolTable.setValueFor(variableName, expression->evaluate(symbolTable));
}

void AssignmentStatement::print() const {
    std::cout << variableName << " = ";
    expression->print();
    std::cout << '\n';
}

PrintStatement::PrintStatement(ExprNode *expression) {
    this->expression = expression;
}

PrintStatement::~PrintStatement() {
    delete expression;
}

void PrintStatement::evaluate(SymbolTable &symbolTable) const {
    std::cout << expression->evaluate(symbolTable) << '\n';
}

void PrintStatement::print() const {
    std::cout << "print ";
    expression->print();
    std::cout << '\n';
}

ForStatement::ForStatement(AssignmentStatement* initializer, ExprNode* condition, AssignmentStatement* update, Statements* body) {
    this->initializer = initializer;
    this->condition = condition;
    this->update = update;
    this->body = body;
}

ForStatement::~ForStatement() {
    delete initializer;
    delete condition;
    delete update;
    delete body;
}

void ForStatement::evaluate(SymbolTable &symbolTable) const {
    for (initializer->evaluate(symbolTable); condition->evaluate(symbolTable) != 0; update->evaluate(symbolTable)) {
        body->evaluate(symbolTable);
    }
}

void ForStatement::print() const {
    std::cout << "for (";
    initializer->print();
    std::cout << "; ";
    condition->print();
    std::cout << "; ";
    update->print();
    std::cout << ") {\n";
    body->print();
    std::cout << "\n}\n";
}
