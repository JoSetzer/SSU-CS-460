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

ForStatement::ForStatement(std::string itterVar, RangeExpression *range, Statements* body) {
    this->itterVar = itterVar;
    this->range = range;
    this->body = body;
}

ForStatement::~ForStatement() {
    delete range;
    delete body;
}

void ForStatement::evaluate(SymbolTable &symbolTable) const {
	EvaluatedRange evaluated = range->evaluate(symbolTable);

	if (!evaluated.hasIteration())
		return;

	int nextValue = evaluated.start();

	while (true) {
		symbolTable.setValueFor(itterVar, nextValue);

		body->evaluate(symbolTable);

		nextValue += evaluated.step();

		if (!evaluated.shouldContinue(nextValue))
			break;
	}
}	

void ForStatement::print() const {
    std::cout << "for ";
    std::cout << itterVar;
    std::cout << " in ";
    range->print(std::cout);
    std::cout << ":\n";
    body->print();
    std::cout << "\n";
}
