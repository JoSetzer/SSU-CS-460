#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>

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
	ValueDescriptor *value = expression->evaluate(symbolTable);
	std::cout << value->toString() << '\n';
	delete value;
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

	std::int64_t result = static_cast<IntegerValueDescriptor*>(evaluated.start())->value();

	while (true) {
		symbolTable.setValueFor(itterVar, new IntegerValueDescriptor(result));

		body->evaluate(symbolTable);

		result += static_cast<IntegerValueDescriptor*>(evaluated.step())->value();

		if (!evaluated.shouldContinue(new IntegerValueDescriptor(result)))
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

WhileStatement::WhileStatement(ExprNode *condition, Statements *body) {
    this->condition = condition;
    this->body = body;
}

WhileStatement::~WhileStatement() {
    delete condition;
    delete body;
}

void WhileStatement::evaluate(SymbolTable &symbolTable) const {
    // this is a loop

    while (true) {
        // making sure that the condition is a boolean type
        ValueDescriptor *value = condition->evaluate(symbolTable);
        bool conditionResult = false;

        if (value->type() == ValueDescriptor::Type::boolean) {
            if (value->toString() == "True") {
                conditionResult = true;
            }
        }
        else {
            std::string message = "The \"while\" condition is not a boolean, got " + value->typeName();
            delete value;
            throw std::runtime_error(message);
        }
        delete value;


        if (!conditionResult) {
            return;
        }
        body->evaluate(symbolTable);
    }
}

void WhileStatement::print() const {
    std::cout << "while ";
    condition->print();
    std::cout << ":\n";
    body->print();
    std::cout << "\n";
}

IfStatement::IfStatement(ExprNode *condition, Statements *body) {
    this->condition = condition;
    this->body = body;
    elseStatement = nullptr;
}

IfStatement::IfStatement(ExprNode *condition, Statements *body, ElseStatement *elseStatement) {
    this->condition = condition;
    this->body = body;
    this->elseStatement = elseStatement;
}

IfStatement::IfStatement(ExprNode *condition, Statements *body, std::vector<ElifStatement *> &elifStatements, ElseStatement *elseStatement) {
    this->condition = condition;
    this->body = body;
    this->elifStatements = elifStatements;
    this->elseStatement = elseStatement;
}

IfStatement::~IfStatement() {
    delete condition;
    delete body;
    for (const auto *elifStatement : elifStatements)
        delete elifStatement;
    delete elseStatement;
}

void IfStatement::evaluate(SymbolTable &symbolTable) const {

    ValueDescriptor *value = condition->evaluate(symbolTable);
    bool conditionResult = false;

    if (value->type() == ValueDescriptor::Type::boolean) {
        if (value->toString() == "True") {
            conditionResult = true;
        }
    }
    else {
        std::string message = "The \"if\" condition is not a boolean, got " + value->typeName();
        delete value;
        throw std::runtime_error(message);
    }
    delete value;

    // if (condition == true) evaluate body
    if (conditionResult) {
        body->evaluate(symbolTable);
        return;
    }

    // check whether each elif condition is true and if so return
    for (const auto *elifStatement : elifStatements) {
        if (elifStatement->didEvaluate(symbolTable)) {
            return;
        }
    }

    // if an else exists then evaluate its body
    if (elseStatement != nullptr) {
        elseStatement->evaluate(symbolTable);
    }
}

void IfStatement::print() const {
    std::cout << "if ";
    condition->print();
    std::cout << ":\n";
    body->print();
    std::cout << "\n";

    for (const auto *elifStatement : elifStatements) {
        elifStatement->print();
    }

    if (elseStatement != nullptr) {
        elseStatement->print();
    }
}

ElifStatement::ElifStatement(ExprNode *condition, Statements *body) {
    this->condition = condition;
    this->body = body;
}

ElifStatement::~ElifStatement() {
    delete condition;
    delete body;
}

void ElifStatement::evaluate(SymbolTable &symbolTable) const {
    ValueDescriptor *value = condition->evaluate(symbolTable);
    bool conditionResult = false;

    if (value->type() == ValueDescriptor::Type::boolean) {
        if (value->toString() == "True") {
            conditionResult = true;
        }
    }
    else {
        std::string message = "The \"elif\" condition is not a boolean, got " + value->typeName();
        delete value;
        throw std::runtime_error(message);
    }
    delete value;

    if (conditionResult) {
        body->evaluate(symbolTable);
    }
}

bool ElifStatement::didEvaluate(SymbolTable &symbolTable) const {
    ValueDescriptor *value = condition->evaluate(symbolTable);
    bool conditionResult = false;

    if (value->type() == ValueDescriptor::Type::boolean) {
        if (value->toString() == "True") {
            conditionResult = true;
        }
    }
    else {
        std::string message = "The \"elif\" condition is not a boolean, got " + value->typeName();
        delete value;
        throw std::runtime_error(message);
    }
    delete value;

    if (conditionResult) {
        body->evaluate(symbolTable);
    }
    return conditionResult;
}

void ElifStatement::print() const {
    std::cout << "elif ";
    condition->print();
    std::cout << ":\n";
    body->print();
    std::cout << "\n";
}

ElseStatement::ElseStatement(Statements *body) {
    this->body = body;
}

ElseStatement::~ElseStatement() {
    delete body;
}

void ElseStatement::evaluate(SymbolTable &symbolTable) const {
    body->evaluate(symbolTable);
}

void ElseStatement::print() const {
    std::cout << "else:\n";
    body->print();
}
