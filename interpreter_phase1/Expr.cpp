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

ValueDescriptor *BinaryExprNode::evaluate(const SymbolTable &symbolTable) const {
    ValueDescriptor* leftValue = leftOperand->evaluate(symbolTable);
    ValueDescriptor* rightValue = nullptr;

    ValueDescriptor *result = nullptr;

    try {
        rightValue = rightOperand->evaluate(symbolTable);

        if (token().isAdditionOperator())
            result = additionOperation(leftValue, rightValue);
        if (token().isSubtractionOperator())
            result = subtractionOperation(leftValue, rightValue);
        if (token().isMultiplicationOperator())
            result = multiplicationOperation(leftValue, rightValue);
        if (token().isCompareOperator())
            result = equalsOperation(leftValue, rightValue);
        if (token().isNotEqualOperator())
            result = notEqualsOperation(leftValue, rightValue);
        if (token().isGreaterThanOperator())
            result = greaterOperation(leftValue, rightValue);
        if (token().isGreaterThanOrEqualOperator())
            result = greaterOrEqualsOperation(leftValue, rightValue);
        if (token().isLessThanOperator())
            result = lessOperation(leftValue, rightValue);
        if (token().isLessThanOrEqualOperator())
            result = lessOrEqualsOperation(leftValue, rightValue);
        if (token().isDivisionOperator())
            result = divisionOperation(leftValue, rightValue);
        if (token().isFloorDivisionOperator())
            result = floorDivisionOperation(leftValue, rightValue);
        if (token().isModuloOperator())
            result = moduloOperation(leftValue, rightValue);
    } catch (...) {
        delete leftValue;
        delete rightValue;
        throw;
    }

    delete leftValue;
    delete rightValue;
    return result;
}

ValueDescriptor *BinaryExprNode::additionOperation(ValueDescriptor *left, ValueDescriptor *right) const {

    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    // if both operances are integers
    if (left->type() == integer && right->type() == integer) {
            std::int64_t result = static_cast<const IntegerValueDescriptor*>(left)->value() +
                static_cast<const IntegerValueDescriptor*>(right)->value();
            return new IntegerValueDescriptor(result);
        }
    // one or both operands is a double
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() + r;
        return new DoubleValueDescriptor(result);
    }
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        double result = l + static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }
    if (left->type() == doubleVal && right->type() == doubleVal) {
        double l = static_cast<const DoubleValueDescriptor*>(left)->value();
        double r = static_cast<const DoubleValueDescriptor*>(right)->value();
        double result = l + r;
        return new DoubleValueDescriptor(result);
    }

    // One or both operands is a string
    if (left->type() == stringVal && right->type() == stringVal) {
        std::string l = static_cast<const StringValueDescriptor*>(left)->value();
        std::string r = static_cast<const StringValueDescriptor*>(right)->value();
        std::string result = l + r;
        return new StringValueDescriptor(result);
    }
    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::subtractionOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;

    // both operands are integers
    if (left->type() == integer && right->type() == integer) {
        std::int64_t result = static_cast<const IntegerValueDescriptor*>(left)->value() -
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new IntegerValueDescriptor(result);
    }
    // one or both operands are doubles
    // integer - double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        double result = l - static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }
    // double - integer
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() - r;
        return new DoubleValueDescriptor(result);
    }
    // double - double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() -
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::multiplicationOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;

    // integer * integer
    if (left->type() == integer && right->type() == integer) {
        std::int64_t result = static_cast<const IntegerValueDescriptor*>(left)->value() *
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new IntegerValueDescriptor(result);
    }
    // double * double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() *
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }
    // integer * double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        double result = l * static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }
    // double * integer
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() * r;
        return new DoubleValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::divisionOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;

    if (right->type() ==integer)
        if (static_cast<double>(static_cast<const IntegerValueDescriptor*>(right)->value()) == 0.0)
            throw std::runtime_error("division by zero at line " + std::to_string(token().lineNumber()));

    if (right->type() == doubleVal)
        if (static_cast<const DoubleValueDescriptor*>(right)->value() == 0.0)
            throw std::runtime_error("division by zero at line" + std::to_string(token().lineNumber()));

    // integer / integer
    if (left->type() == integer && right->type() == integer) {
        double result = static_cast<double>(static_cast<const IntegerValueDescriptor*>(left)->value()) /
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }

    // double / double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() /
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }

    //integer / double
    if (left->type() == integer && right->type() == doubleVal) {
        std::int64_t l = static_cast<const IntegerValueDescriptor*>(left)->value();
        double result = l / static_cast<const DoubleValueDescriptor*>(right)->value();
        return new DoubleValueDescriptor(result);
    }

    // double / integer
    if (left->type() == doubleVal && right->type() == integer) {
        std::int64_t r = static_cast<const IntegerValueDescriptor*>(right)->value();
        double result = static_cast<const DoubleValueDescriptor*>(left)->value() / r;
        return new DoubleValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::floorDivisionOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;

    if (right->type() == integer)
        if (static_cast<const IntegerValueDescriptor*>(right)->value() == 0)
            throw std::runtime_error("division by zero at line" + std::to_string(token().lineNumber()));

    // integer // integer
    if (left->type() == integer && right->type() == integer) {
        std::int64_t l = static_cast<const IntegerValueDescriptor*>(left)->value();
        std::int64_t r = static_cast<const IntegerValueDescriptor*>(right)->value();
        std::int64_t result = l / r;
        std::int64_t remainder = l % r;
        if (remainder != 0 && ((remainder < 0) != (r < 0)))
            result -= 1;
        return new IntegerValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::equalsOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type boolVal = ValueDescriptor::Type::boolean;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    // operands are of the same type
    if (left->type() == right->type()) {
        if (left->type() == integer && right->type() == integer) {
            bool result = static_cast<const IntegerValueDescriptor*>(left)->value() == static_cast<const IntegerValueDescriptor*>(right)->value();
            return new BooleanValueDescriptor(result);
        }
        if (left->type() == doubleVal && right->type() == doubleVal) {
            bool result = static_cast<const DoubleValueDescriptor*>(left)->value() == static_cast<const DoubleValueDescriptor*>(right)->value();
            return new BooleanValueDescriptor(result);
        }
        if (left->type() == boolVal && right->type() == boolVal) {
            bool result = static_cast<const BooleanValueDescriptor*>(left)->value() == static_cast<const BooleanValueDescriptor*>(right)->value();
            return new BooleanValueDescriptor(result);
        }
        if (left->type() == stringVal && right->type() == stringVal) {
            bool result = static_cast<const StringValueDescriptor*>(left)->value() == static_cast<const StringValueDescriptor*>(right)->value();
            return new BooleanValueDescriptor(result);
        }
    }

    //operands are a integer and double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        bool result = l == static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        bool result = r == static_cast<const DoubleValueDescriptor*>(left)->value();
        return new BooleanValueDescriptor(result);
    }

    return new BooleanValueDescriptor(false);
}

ValueDescriptor *BinaryExprNode::notEqualsOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type boolVal = ValueDescriptor::Type::boolean;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    //integers
    if (left->type() == integer && right->type() == integer) {
        bool result = static_cast<const IntegerValueDescriptor*>(left)->value() !=
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // doubles
    if (left->type() == doubleVal && right->type() == doubleVal) {
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() !=
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // booleans
    if (left->type() == boolVal && right->type() == boolVal) {
        bool result = static_cast<const BooleanValueDescriptor*>(left)->value() !=
            static_cast<const BooleanValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // strings
    if (left->type() == stringVal && right->type() == stringVal) {
        bool result = static_cast<const StringValueDescriptor*>(left)->value() !=
            static_cast<const StringValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // doubles and integers
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        bool result = l != static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        bool result = r != static_cast<const DoubleValueDescriptor*>(left)->value();
        return new BooleanValueDescriptor(result);
    }

    return new BooleanValueDescriptor(true);
}

ValueDescriptor *BinaryExprNode::moduloOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;

    if (right->type() == integer) {
        if (static_cast<const IntegerValueDescriptor*>(right)->value() == 0) {
            throw std::runtime_error("division by zero at line " + std::to_string(token().lineNumber()));
        }
    }

    // integer % integer
    if (left->type() == integer && right->type() == integer) {
        std::int64_t l = static_cast<const IntegerValueDescriptor*>(left)->value();
        std::int64_t r = static_cast<const IntegerValueDescriptor*>(right)->value();
        std::int64_t result = l % r;
        if (result != 0 && ((result < 0) != (r < 0)))
            result += r;
        return new IntegerValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::lessOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    // integer < integer
    if (left->type() == integer && right->type() == integer) {
        bool result = static_cast<const IntegerValueDescriptor*>(left)->value() <
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double < double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() <
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // integer < double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        bool result = l < static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double < integer
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() < r;
        return new BooleanValueDescriptor(result);
    }

    // string < string
    if (left->type() == stringVal && right->type() == stringVal) {
        bool result = static_cast<const StringValueDescriptor*>(left)->value() <
            static_cast<const StringValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::lessOrEqualsOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    // integer < integer
    if (left->type() == integer && right->type() == integer) {
        bool result = static_cast<const IntegerValueDescriptor*>(left)->value() <=
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double < double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() <=
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // integer < double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        bool result = l <= static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double < integer
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() <= r;
        return new BooleanValueDescriptor(result);
    }

    // string < string
    if (left->type() == stringVal && right->type() == stringVal) {
        bool result = static_cast<const StringValueDescriptor*>(left)->value() <=
            static_cast<const StringValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::greaterOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    // integer > integer
    if (left->type() == integer && right->type() == integer) {
        bool result = static_cast<const IntegerValueDescriptor*>(left)->value() >
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double > double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() >
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // integer > double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        bool result = l > static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double > integer
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() > r;
        return new BooleanValueDescriptor(result);
    }

    // string > string
    if (left->type() == stringVal && right->type() == stringVal) {
        bool result = static_cast<const StringValueDescriptor*>(left)->value() >
            static_cast<const StringValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }

    typeError(left, right);
}

ValueDescriptor *BinaryExprNode::greaterOrEqualsOperation(ValueDescriptor *left, ValueDescriptor *right) const {
    // simplify type comparisons
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    ValueDescriptor::Type stringVal = ValueDescriptor::Type::string;

    // integer >= integer
    if (left->type() == integer && right->type() == integer) {
        bool result = static_cast<const IntegerValueDescriptor*>(left)->value() >=
            static_cast<const IntegerValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double >= double
    if (left->type() == doubleVal && right->type() == doubleVal) {
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() >=
            static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // integer >= double
    if (left->type() == integer && right->type() == doubleVal) {
        double l = static_cast<const IntegerValueDescriptor*>(left)->value();
        bool result = l >= static_cast<const DoubleValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }
    // double >= integer
    if (left->type() == doubleVal && right->type() == integer) {
        double r = static_cast<const IntegerValueDescriptor*>(right)->value();
        bool result = static_cast<const DoubleValueDescriptor*>(left)->value() >= r;
        return new BooleanValueDescriptor(result);
    }

    // string >= string
    if (left->type() == stringVal && right->type() == stringVal) {
        bool result = static_cast<const StringValueDescriptor*>(left)->value() >=
            static_cast<const StringValueDescriptor*>(right)->value();
        return new BooleanValueDescriptor(result);
    }

    typeError(left, right);
}

void BinaryExprNode::typeError(const ValueDescriptor *left, const ValueDescriptor *right) const {
    const char *s = token().symbol();
    std::string op(s, s[1] == '\0' ? 1 : 2);

    std::string message =
    "type error at line " + std::to_string(token().lineNumber()) +
    ": unsupported operand types for '" + op + "': " +
    left->typeName() + " and " + right->typeName();

    throw std::runtime_error(message);
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

ValueDescriptor *UnaryExprNode::evaluate(const SymbolTable &symbolTable) const {
    ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
    ValueDescriptor::Type doubleVal = ValueDescriptor::Type::doubleValue;
    
    const ValueDescriptor *value = expression->evaluate(symbolTable);
    
    if (token().isAdditionOperator()) {
	if (value->type() == integer) {
		std::int64_t result = static_cast<const IntegerValueDescriptor*>(value)->value();
		delete value;
		return new IntegerValueDescriptor(result);
	}
	if (value->type() == doubleVal) {
		double result = static_cast<const DoubleValueDescriptor*>(value)->value();
		delete value;
		return new DoubleValueDescriptor(result);
	}
    }
    
    if (token().isSubtractionOperator()) {
	if (value->type() == integer) {
		std::int64_t result = static_cast<const IntegerValueDescriptor*>(value)->value();
		delete value;
    		return new IntegerValueDescriptor(-result);
	}
	if (value->type() == doubleVal) {
		double result = static_cast<const DoubleValueDescriptor*>(value)->value();
		delete value;
		return new DoubleValueDescriptor(-result);
	}
    }
    
    delete value;
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

ValueDescriptor *IntegerLiteral::evaluate(const SymbolTable &) const {
    return new IntegerValueDescriptor(token().integerValue());
}

DoubleLiteral::DoubleLiteral(Token token) : ExprNode{std::move(token)} {}

void DoubleLiteral::print() const {
    token().print(std::cout);
}

ValueDescriptor *DoubleLiteral::evaluate(const SymbolTable &) const {
    return new DoubleValueDescriptor(token().doubleValue());
}

StringLiteral::StringLiteral(Token token) : ExprNode{std::move(token)} {}

void StringLiteral::print() const {
    token().print(std::cout);
}

ValueDescriptor *StringLiteral::evaluate(const SymbolTable &) const {
    return new StringValueDescriptor(token().stringValue());
}

BoolLiteral::BoolLiteral(Token token) : ExprNode{std::move(token)} {}

void BoolLiteral::print() const {
    token().print(std::cout);
}

ValueDescriptor *BoolLiteral::evaluate(const SymbolTable &) const {
    return new BooleanValueDescriptor(token().booleanValue());
}

StringConversion::StringConversion(Token token, ExprNode *operand): ExprNode{std::move(token)}, literal(operand) {}

StringConversion::~StringConversion() {
    delete literal;
}

ValueDescriptor *StringConversion::evaluate(const SymbolTable &symbolTable) const {
    ValueDescriptor* str = literal->evaluate(symbolTable);

    std::string result = str->toString();

    delete str;

    return new StringValueDescriptor(std::move(result));
}

void StringConversion::print() const {
    std::cout << "str(";
    literal->print();
    std::cout << ')';
}

Variable::Variable(Token token) : ExprNode{std::move(token)} {}

void Variable::print() const {
    token().print(std::cout);
}

ValueDescriptor *Variable::evaluate(const SymbolTable &symbolTable) const {

    if (!symbolTable.isDefined(token().identifier()))
        throw std::runtime_error("Unknown variable: " + token().identifier());

    return symbolTable.getValueFor(token().identifier())->clone();
}

EvaluatedRange::EvaluatedRange(ValueDescriptor *start, ValueDescriptor *stop, ValueDescriptor *step):
				start_(start), stop_(stop), step_(step) {
    	if(static_cast<IntegerValueDescriptor*>(step_)->value() == 0) {
		delete start_;
		delete stop_;
		delete step_;
		throw std::logic_error("error: zero-value step");
	}
}

ValueDescriptor *EvaluatedRange::start() const {return start_;}

ValueDescriptor *EvaluatedRange::stop() const {return stop_;}

ValueDescriptor *EvaluatedRange::step() const {return step_;}

EvaluatedRange::~EvaluatedRange () {
	delete start_;
	delete stop_;
	delete step_;
}

bool EvaluatedRange::hasIteration() const {
	std::int64_t start, stop, step;
	start = static_cast<IntegerValueDescriptor*>(start_)->value();
	stop = static_cast<IntegerValueDescriptor*>(stop_)->value();
	step = static_cast<IntegerValueDescriptor*>(step_)->value();
	
	if (step > 0)
		return start < stop;
	return start > stop;
}

bool EvaluatedRange::shouldContinue (ValueDescriptor *nextValue_) const {
	std::int64_t step, stop, nextValue;
	step = static_cast<IntegerValueDescriptor*>(step_)->value();
	stop = static_cast<IntegerValueDescriptor*>(stop_)->value();
	nextValue = static_cast<IntegerValueDescriptor*>(nextValue_)->value();
	delete nextValue_;

	if (step > 0)
		return nextValue < stop;
	return nextValue > stop;
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
	ValueDescriptor *start, *stop, *step;
    	ValueDescriptor::Type integer = ValueDescriptor::Type::integer;
	
	if (startExpression != nullptr) {
		start = startExpression->evaluate(symbolTable);
		if (start->type() != integer) {
			delete start;
       			throw std::runtime_error("RangeExpression::evaluate non-integer range value");
		}
	} else
		start = new IntegerValueDescriptor(0);

	if (stepExpression != nullptr) { 
		step = stepExpression->evaluate(symbolTable);
		if (step->type() != integer) {
       			delete start;
			delete step;
			throw std::runtime_error("RangeExpression::evaluate non-integer range value");
		}
	} else
		step = new IntegerValueDescriptor(1);

	stop = stopExpression->evaluate(symbolTable);
	if (stop->type() != integer) {
       		delete start;
		delete stop;
		delete step;
		throw std::runtime_error("RangeExpression::evaluate non-integer range value");
	}

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
