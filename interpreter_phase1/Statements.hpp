#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <string>
#include <vector>

#include "Expr.hpp"
#include "SymbolTable.hpp"
#include "ValueDescriptor.h"

class Statement {
public:
    virtual ~Statement() = default;

    virtual void print() const = 0;
    virtual void evaluate(SymbolTable &symbolTable) const = 0;
};

class Statements {
public:
    ~Statements();

    void addStatement(Statement *statement);
    void evaluate(SymbolTable &symbolTable) const;
    void print() const;

private:
    std::vector<Statement *> statements;
};

class AssignmentStatement final : public Statement {
public:
    AssignmentStatement(std::string variableName,
                        ExprNode *expression);
    ~AssignmentStatement() override;

    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;

private:
    std::string variableName;
    ExprNode *expression;
};

class PrintStatement final : public Statement {
    public:
    explicit PrintStatement(ExprNode *expression);
    ~PrintStatement() override;

    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;
    private:
    ExprNode *expression;
};


class ForStatement final : public Statement {
    public:
    // ForStatement(AssignmentStatement* initializer, ExprNode* condition, AssignmentStatement* update, Statements* body);
    ForStatement(std::string itterVar, RangeExpression *range, Statements* body);
    ~ForStatement() override;
    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;

    private:
    // AssignmentStatement* initializer;
    // ExprNode* condition;
    // AssignmentStatement* update;
    std::string itterVar;
    RangeExpression* range;
    Statements* body;
};

class WhileStatement final : public Statement {
    public:
    WhileStatement(ExprNode *condition, Statements *body);
    ~WhileStatement() override;
    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;
    private:
    ExprNode *condition;
    Statements *body;
};

class ElifStatement final : public Statement {
    public:
    ElifStatement(ExprNode *condition, Statements *body);
    ~ElifStatement() override;
    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;
    bool didEvaluate(SymbolTable &symbolTable) const;

    private:
    ExprNode *condition;
    Statements *body;
};

class ElseStatement final : public Statement {
    public:
    ElseStatement(Statements *body);
    ~ElseStatement() override;
    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;

    private:
    Statements *body;
};

class IfStatement final : public Statement {
    public:
    IfStatement(ExprNode *condition, Statements *body);
    IfStatement(ExprNode *condition, Statements *body, ElseStatement *elseStatement);
    IfStatement(ExprNode *condition, Statements *body, std::vector<ElifStatement* > &elifStatements, ElseStatement *elseStatement);

    ~IfStatement() override;

    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;



    private:
    ExprNode *condition;
    Statements* body;
    std::vector<ElifStatement *> elifStatements;
    ElseStatement *elseStatement;
};



#endif // EXPRINTER_STATEMENTS_HPP
