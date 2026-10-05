#include <cstdlib>
#include <iostream>
#include <string>

#include "Parser.hpp"

void Parser::die(const std::string &where,
                 const std::string &message,
                 const Token &token) const {
    std::cerr << where << ": " << message << " at line " << token.lineNumber()
              << ", column " << token.columnNumber() << ". Got: ";
    token.print(std::cerr);
    std::cerr << "\n\nTokens identified up to this point:\n";
    tokenizer.printProcessedTokens(std::cerr);
    std::exit(EXIT_FAILURE);
}

Statements *Parser::program() {
    // <program> -> <statements> EOF
    Statements *parsedStatements = statements();
    Token eof = tokenizer.getToken();
    if (!eof.isEof()) {
        delete parsedStatements;
        die("Parser::program", "expected EOF", eof);
    }
    return parsedStatements;
}

Statements *Parser::statements() {
    // <statements> -> <statement> NEWLINE { <statement> NEWLINE }
    auto *parsedStatements = new Statements();
    parsedStatements->addStatement(statement());

    // Token newline = tokenizer.getToken();
    // if (!newline.isNewline()) {
    //     delete parsedStatements;
    //     die("Parser::statements", "expected NEWLINE after statement", newline);
    // }


    Token next = tokenizer.getToken();
    while (!next.isDedent() && !next.isEof()) {
        tokenizer.ungetToken();
        parsedStatements->addStatement(statement());
        next = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return parsedStatements;
}

Statement *Parser::statement() {
    // <statement> -> <for-statement>
    //             | <assignment-statement>
    //             | <print-statement>
    Token token = tokenizer.getToken();

    if (token.isIdentifier()) {
        tokenizer.ungetToken();
        Statement* assignment = assignmentStatement();

        Token newline = tokenizer.getToken();
        if (!newline.isNewline())
            die("Parser::statement", "expected NEWLINE after statement", newline);

        return assignment;
    }
    if (token.isForKeyword())
        return forStatement();

    if (token.isPrintKeyword()) {
        Statement* print = new PrintStatement(relExpr());

        Token newline = tokenizer.getToken();
        if (!newline.isNewline())
            die("Parser::statement", "expected NEWLINE after statement", newline);

        return print;
    }

    if (token.isIndent()) {
        die("Parser::statement", "unexpected INDENT", token);
    }

    die("Parser::statement", "expected a statement", token);
}

Statements *Parser::suite() {
    Token token = tokenizer.getToken();

    if (!token.isNewline()) {
        die("Parser::suite", "expected NEWLINE", token);
    }

    token = tokenizer.getToken();
    if (!token.isIndent()) {
        die("Parser::suite", "expected INDENT", token);
    }

    Statements *body = statements();

    token = tokenizer.getToken();
    if (!token.isDedent()) {
        die("Parser::suite", "expected DEDENT", token);
    }

    return body;
}


AssignmentStatement *Parser::assignmentStatement() {
    // <assignment-statement> -> <id> = <rel-expr>
    // The caller consumes the context-dependent terminator: NEWLINE in a
    // statement list or ';' in a future for-loop header.
    Token variable = tokenizer.getToken();
    if (!variable.isIdentifier())
        die("Parser::assignmentStatement", "expected an identifier", variable);

    Token assignmentOperator = tokenizer.getToken();
    if (!assignmentOperator.isAssignmentOperator())
        die("Parser::assignmentStatement", "expected '='", assignmentOperator);

    return new AssignmentStatement(variable.identifier(), relExpr());
}

ForStatement* Parser::forStatement() {
    Token openParen = tokenizer.getToken();

    if (!openParen.isOpenParen()) {
        die("Parser::forStatement", "expected '(", openParen);
    }
    AssignmentStatement* initializer = assignmentStatement();

    Token semiCol = tokenizer.getToken();

    if (!semiCol.isSemicolon()) {
        die("Parser::forStatement", "expected ';'", semiCol);
    }

    ExprNode* condition = relExpr();

    Token semiCol2 = tokenizer.getToken();
    if (!semiCol2.isSemicolon()) {
        die("Parser::forStatement", "expected ';'", semiCol2);
    }

    AssignmentStatement* update = assignmentStatement();

    Token endParen = tokenizer.getToken();
    if (!endParen.isCloseParen()) {
        die("Parser::forStatement", "expected ')'", endParen);
    }

    Token colon = tokenizer.getToken();
    if (!colon.isColon()) {
        die("Parser::forStatement", "expected ':'", colon);
    }

    Statements* body = suite();

    // Token openBrace = tokenizer.getToken();
    // if (!openBrace.isOpenBrace()) {
    //     die("Parser::forStatement", "expected '{'", openBrace);
    // }
    //
    // Token newLine = tokenizer.getToken();
    // if (!newLine.isNewline()) {
    //     die("Parser::forStatement", "expected ''", newLine);
    // }
    //
    // Statements* body = statements();
    //
    // Token closeBrace = tokenizer.getToken();
    // if (!closeBrace.isCloseBrace()) {
    //     die("Parser::forStatement", "expected '}'", closeBrace);
    // }

    return new ForStatement(initializer, condition, update, body);
}

ExprNode *Parser::relExpr() {
    // <rel-expr> -> <rel-term> [ <equality-op> <rel-term> ]
    // The optional equality operation is left for students to implement.
	ExprNode* left = relTerm();
	Token token = tokenizer.getToken();
    	
	if (token.isCompareOperator() || token.isNotEqualOperator()){
		ExprNode *right = relTerm();
		left = new BinaryExprNode(token, left, right);
		token = tokenizer.getToken();
	}	
	if (token.isCompareOperator() || token.isNotEqualOperator()){
            die("Parser::relExpr", "too many equality-ops", token);
	}

	tokenizer.ungetToken();
	return left;
}

ExprNode *Parser::relTerm() {
    // <rel-term> -> <rel-primary> [ <ordering-op> <rel-primary> ]
    // The optional ordering operation is left for students to implement.
	ExprNode* left = relPrimary();
	Token token = tokenizer.getToken();
	
	if (token.isGreaterThanOperator() || token.isGreaterThanOrEqualOperator() ||
	    token.isLessThanOperator() || token.isLessThanOrEqualOperator()) {
		ExprNode *right = relPrimary();
		left = new BinaryExprNode(token, left, right);
		token = tokenizer.getToken();
	}
	if (token.isGreaterThanOperator() || token.isGreaterThanOrEqualOperator() ||
	    token.isLessThanOperator() || token.isLessThanOrEqualOperator()) {
            die("Parser::relTerm", "too many ordering-ops", token);
	}

	tokenizer.ungetToken();
       	return left;
}

ExprNode *Parser::relPrimary() {
    // <rel-primary> -> <arith-expr>
    return arithExpr();
}

ExprNode *Parser::arithExpr() {
    // <arith-expr> -> <arith-term> { <add-op> <arith-term> }
    ExprNode *left = arithTerm();
    Token token = tokenizer.getToken();

    while (token.isAdditionOperator() || token.isSubtractionOperator()) {
        ExprNode *right = arithTerm();
        left = new BinaryExprNode(token, left, right);
        token = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arithTerm() {
    // <arith-term> -> <arith-primary> { <mult-op> <arith-primary> }
    ExprNode *left = arithPrimary();
    Token token = tokenizer.getToken();

    while (token.isMultiplicationOperator() ||
           token.isDivisionOperator() ||
           token.isModuloOperator()) {
        ExprNode *right = arithPrimary();
        left = new BinaryExprNode(token, left, right);
        token = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arithPrimary() {
    // <arith-primary> -> [ <sign> ] <arith-atom>
    Token token = tokenizer.getToken();
    if (token.isAdditionOperator() || token.isSubtractionOperator())
        return new UnaryExprNode(token, arithAtom());

    tokenizer.ungetToken();
    return arithAtom();
}

ExprNode *Parser::arithAtom() {
    // <arith-atom> -> <id> | <integer> | '(' <rel-expr> ')'
    Token token = tokenizer.getToken();

    if (token.isInteger())
        return new IntegerLiteral(token);
    if (token.isIdentifier())
        return new Variable(token);
    if (token.isOpenParen()) {
        ExprNode *expression = relExpr();
        Token closeParen = tokenizer.getToken();
        if (!closeParen.isCloseParen())
            die("Parser::arithAtom", "expected ')'", closeParen);
        return expression;
    }

    die("Parser::arithAtom", "expected an identifier, integer, or '('", token);
}
