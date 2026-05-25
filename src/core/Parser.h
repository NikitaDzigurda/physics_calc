#pragma once

#include "Lexer.h"
#include "Quantity.h"

#include <string>
#include <vector>

namespace pc {

class Calculator;

// Recursive-descent parser/evaluator with grammar:
//
//   stmt    := IDENT '=' expr | expr
//   expr    := term     (('+'|'-') term)*
//   term    := factor   (('*'|'/') factor | implicit-mul factor)*
//   factor  := unary    ('^' factor)?               // right-assoc
//   unary   := ('+'|'-') unary | atom
//   atom    := NUMBER | IDENT | '(' expr ')'
//
// Implicit multiplication kicks in when a primary (atom or close-paren) is
// followed by another primary without an operator (e.g. "9.8 m/s^2").
//
// Evaluation result is a Quantity<double>. If the input was an assignment,
// the calculator binds the result to the identifier and the result is
// returned with the binding side effect.
class Parser {
public:
    Parser(std::vector<Token> tokens, Calculator& calc);

    struct Result {
        Q           value;
        bool        wasAssignment = false;
        std::string assignedName;
    };

    Result parse(); // throws ParseError / EvalError

private:
    const Token& peek(int off = 0) const { return tokens_[i_ + off]; }
    const Token& consume();
    bool accept(Tok k);
    void expect(Tok k, const char* what);

    Q parseExpr();
    Q parseTerm();
    Q parseFactor();
    Q parseUnary();
    Q parseAtom();

    bool atPrimaryStart() const;

    std::vector<Token> tokens_;
    std::size_t        i_ = 0;
    Calculator&        calc_;
};

}
