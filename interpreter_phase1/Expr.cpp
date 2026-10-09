#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "Expr.hpp"

ExprNode::ExprNode(Token token) : expressionToken{std::move(token)} {}

const Token &ExprNode::token() const {
    return expressionToken;
}

BinaryExprNode::BinaryExprNode(Token token, ExprNode *left, ExprNode *right)
    : ExprNode{std::move(token)},
      leftOperand{left},
      rightOperand{right} {}

BinaryExprNode::~BinaryExprNode() {
    delete leftOperand;
    delete rightOperand;
}

int BinaryExprNode::evaluate(const SymbolTable &symbolTable) const {
    const int leftValue = leftOperand->evaluate(symbolTable);
    const int rightValue = rightOperand->evaluate(symbolTable);

    if (token().isAdditionOperator())
        return leftValue + rightValue;
    if (token().isSubtractionOperator())
        return leftValue - rightValue;
    if (token().isMultiplicationOperator())
        return leftValue * rightValue;
    if (token().isCompareOperator())
        return leftValue == rightValue;
    if (token().isNotEqualOperator())
        return leftValue != rightValue;
    if (token().isGreaterThanOperator())
        return leftValue > rightValue;
    if (token().isGreaterThanOrEqualOperator())
        return leftValue >= rightValue;
    if (token().isLessThanOperator())
        return leftValue < rightValue;
    if (token().isLessThanOrEqualOperator())
        return leftValue <= rightValue;
    if (token().isDivisionOperator()) {
        if (rightValue == 0)
            throw std::runtime_error(
                "division by zero at line " + std::to_string(token().lineNumber()) +
                ", column " + std::to_string(token().columnNumber()));
        return leftValue / rightValue;
    }
    if (token().isModuloOperator()) {
        if (rightValue == 0)
            throw std::runtime_error(
                "modulo by zero at line " + std::to_string(token().lineNumber()) +
                ", column " + std::to_string(token().columnNumber()));
        return leftValue % rightValue;
    }

    throw std::logic_error("unsupported infix operator");
}

void BinaryExprNode::print() const {
    std::cout << '(';
    leftOperand->print();
    token().print(std::cout);
    rightOperand->print();
    std::cout << ')';
}

UnaryExprNode::UnaryExprNode(Token token, ExprNode *operand)
    : ExprNode{std::move(token)}, expression{operand} {}

UnaryExprNode::~UnaryExprNode() {
    delete expression;
}

int UnaryExprNode::evaluate(const SymbolTable &symbolTable) const {
    const int value = expression->evaluate(symbolTable);
    if (token().isAdditionOperator())
        return value;
    if (token().isSubtractionOperator())
        return -value;
    throw std::logic_error("unsupported prefix operator");
}

void UnaryExprNode::print() const {
    std::cout << token().symbol();
    expression->print();
}

IntegerLiteral::IntegerLiteral(Token token) : ExprNode{std::move(token)} {}

void IntegerLiteral::print() const {
    token().print(std::cout);
}

int IntegerLiteral::evaluate(const SymbolTable &) const {
    return token().integerValue();
}

Variable::Variable(Token token) : ExprNode{std::move(token)} {}

void Variable::print() const {
    token().print(std::cout);
}

int Variable::evaluate(const SymbolTable &symbolTable) const {
    return symbolTable.getValueFor(token().identifier());
}

EvaluatedRange::EvaluatedRange(int start, int stop, int step):
				start_(start), stop_(stop), step_(step) {
    	if(step_ == 0)
		throw std::logic_error("error: zero-value step");
}

int EvaluatedRange::start() const {return start_;}

int EvaluatedRange::stop() const {return stop_;}

int EvaluatedRange::step() const {return step_;}

bool EvaluatedRange::hasIteration() const {
	if (step_ > 0)
		return start_ < stop_;
	return start_ > stop_;
}

bool EvaluatedRange::shouldContinue (int nextValue) const {
	if (step_ > 0)
		return nextValue < stop_;
	return nextValue > stop_;
}

RangeExpression::RangeExpression(ExprNode *stop):
	startExpression(NULL),
	stopExpression(stop),
	stepExpression(NULL)
	{}
RangeExpression::RangeExpression(ExprNode *start, ExprNode *stop):
	startExpression(start),
	stopExpression(stop),
	stepExpression(NULL)
	{}
RangeExpression::RangeExpression(
	ExprNode *start,
	ExprNode *stop,
	ExprNode *step
	): 
	startExpression(start),
	stopExpression(stop),
	stepExpression(step)
	{}

RangeExpression::~RangeExpression(){
	delete startExpression;
	delete stopExpression;
	delete stepExpression;
}

EvaluatedRange RangeExpression::evaluate(const SymbolTable& symbolTable) const {
	int start = 0;
	int step = 1;
	if (startExpression != nullptr) {
		start = startExpression->evaluate(symbolTable);
	}

	if (stepExpression != nullptr) {
		step = stepExpression->evaluate(symbolTable);
	}

	int stop = stopExpression->evaluate(symbolTable);

	return EvaluatedRange(start, stop, step);	
}

void RangeExpression::print(std::ostream& output) const {
    output << "range(";

    if (startExpression == nullptr) {
        stopExpression->print();
    }
    else if (stepExpression == nullptr) {
        startExpression->print();
        output << ", ";
        stopExpression->print();
    }
    else {
        startExpression->print();
        output << ", ";
        stopExpression->print();
        output << ", ";
        stepExpression->print();
    }
    output << ")";
}
