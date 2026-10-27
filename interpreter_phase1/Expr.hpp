#ifndef EXPRINTER_EXPR_HPP
#define EXPRINTER_EXPR_HPP

#include "SymbolTable.hpp"
#include "Token.hpp"
#include "ValueDescriptor.h"

class ExprNode {
public:
    explicit ExprNode(Token token);
    virtual ~ExprNode() = default;

    [[nodiscard]] const Token &token() const;
    virtual void print() const = 0;
    [[nodiscard]] virtual ValueDescriptor *evaluate(const SymbolTable &symbolTable) const = 0;

private:
    Token expressionToken;
};

class BinaryExprNode final : public ExprNode {
public:
    BinaryExprNode(Token token, ExprNode *left, ExprNode *right);
    ~BinaryExprNode() override;

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;
    [[nodiscard]] ValueDescriptor *additionOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *subtractionOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *multiplicationOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *divisionOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *floorDivisionOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *equalsOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *notEqualsOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *lessOrEqualsOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *lessOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *greaterOrEqualsOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *greaterOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[nodiscard]] ValueDescriptor *moduloOperation(ValueDescriptor *left, ValueDescriptor *right) const;
    [[noreturn]] void typeError(const ValueDescriptor *left, const ValueDescriptor *right) const;


private:
    ExprNode *leftOperand;
    ExprNode *rightOperand;
};

class UnaryExprNode final : public ExprNode {
public:
    UnaryExprNode(Token token, ExprNode *operand);
    ~UnaryExprNode() override;

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;

private:
    ExprNode *expression;
};

class IntegerLiteral final : public ExprNode {
public:
    explicit IntegerLiteral(Token token);

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;
};

class DoubleLiteral final : public ExprNode {
    public:
    explicit DoubleLiteral(Token token);

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;
};

class BoolLiteral final : public ExprNode {
    public:
    explicit BoolLiteral(Token token);

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;
};

class StringLiteral final : public ExprNode {
    public:
    explicit StringLiteral(Token token);

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;
};

class StringConversion final : public ExprNode {
    public:
    explicit StringConversion(Token token, ExprNode *operand);
    ~StringConversion() override;

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;

    private:
    ExprNode *literal;
};

class Variable final : public ExprNode {
public:
    explicit Variable(Token token);

    void print() const override;
    [[nodiscard]] ValueDescriptor *evaluate(const SymbolTable &symbolTable) const override;
};

class EvaluatedRange {
public:
    EvaluatedRange(ValueDescriptor *start, ValueDescriptor *stop, ValueDescriptor *step);
    ~EvaluatedRange();

    [[nodiscard]] ValueDescriptor *start() const;
    [[nodiscard]] ValueDescriptor *stop() const;
    [[nodiscard]] ValueDescriptor *step() const;

    [[nodiscard]] bool hasIteration() const;
    [[nodiscard]] bool shouldContinue(ValueDescriptor *nextValue_) const;

private:
    ValueDescriptor *start_;
    ValueDescriptor *stop_;
    ValueDescriptor *step_;
};

class RangeExpression {
public:
    explicit RangeExpression(ExprNode* stop);
    RangeExpression(ExprNode* start, ExprNode* stop);
    RangeExpression(
        ExprNode* start,
        ExprNode* stop,
        ExprNode* step
    );
    ~RangeExpression();

    RangeExpression(const RangeExpression&) = delete;
    RangeExpression& operator=(const RangeExpression&) = delete;

    [[nodiscard]] EvaluatedRange evaluate(
        const SymbolTable& symbolTable
    ) const;

    void print(std::ostream& output) const;

private:
    ExprNode* startExpression;
    ExprNode* stopExpression;
    ExprNode* stepExpression;
};
#endif // EXPRINTER_EXPR_HPP
