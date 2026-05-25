#include "Parser.h"

#include "Calculator.h"
#include "Exceptions.h"

#include <cmath>

namespace pc {

Parser::Parser(std::vector<Token> tokens, Calculator& calc)
    : tokens_(std::move(tokens)), calc_(calc) {}

const Token& Parser::consume() { return tokens_[i_++]; }

bool Parser::accept(Tok k) {
    if (peek().kind == k) { ++i_; return true; }
    return false;
}

void Parser::expect(Tok k, const char* what) {
    if (peek().kind != k) {
        throw ParseError(std::string("Expected ") + what +
                         " but got '" + peek().text + "'",
                         peek().pos);
    }
    ++i_;
}

bool Parser::atPrimaryStart() const {
    auto k = peek().kind;
    return k == Tok::Number || k == Tok::Ident || k == Tok::LParen;
}

Parser::Result Parser::parse() {
    Result r;

    if (peek().kind == Tok::Ident && peek(1).kind == Tok::Assign) {
        std::string name = consume().text;
        consume(); // '='
        r.value = parseExpr();
        if (peek().kind != Tok::End) {
            throw ParseError("Trailing input after expression", peek().pos);
        }
        calc_.assign(name, r.value);
        r.wasAssignment = true;
        r.assignedName  = name;
        return r;
    }

    r.value = parseExpr();
    if (peek().kind != Tok::End) {
        throw ParseError("Trailing input after expression", peek().pos);
    }
    return r;
}

Q Parser::parseExpr() {
    Q lhs = parseTerm();
    while (peek().kind == Tok::Plus || peek().kind == Tok::Minus) {
        Tok op = consume().kind;
        Q rhs = parseTerm();
        lhs = (op == Tok::Plus) ? (lhs + rhs) : (lhs - rhs);
    }
    return lhs;
}

Q Parser::parseTerm() {
    Q lhs = parseFactor();
    while (true) {
        if (peek().kind == Tok::Star) {
            consume();
            lhs = lhs * parseFactor();
        } else if (peek().kind == Tok::Slash) {
            consume();
            lhs = lhs / parseFactor();
        } else if (atPrimaryStart()) {
            // implicit multiplication: "9.8 m/s^2", "2 pi", "kg m"
            lhs = lhs * parseFactor();
        } else {
            break;
        }
    }
    return lhs;
}

Q Parser::parseFactor() {
    Q base = parseUnary();
    if (peek().kind == Tok::Caret) {
        consume();
        // right-associative; exponent itself can be unary/parenthesized.
        Q exp = parseFactor();
        if (!exp.dims().isScalar()) {
            throw EvalError("Exponent must be dimensionless");
        }
        double e = exp.value();
        if (base.dims().isScalar()) {
            return Q(std::pow(base.value(), e));
        }
        double rounded = std::round(e);
        if (std::abs(e - rounded) > 1e-9) {
            throw EvalError("Exponent for a dimensioned value must be integer");
        }
        return base.pow(static_cast<int>(rounded));
    }
    return base;
}

Q Parser::parseUnary() {
    if (peek().kind == Tok::Plus)  { consume(); return parseUnary(); }
    if (peek().kind == Tok::Minus) { consume(); return -parseUnary(); }
    return parseAtom();
}

Q Parser::parseAtom() {
    const Token& t = peek();
    if (t.kind == Tok::Number) { consume(); return Q(t.number); }
    if (t.kind == Tok::Ident)  { consume(); return calc_.lookup(t.text); }
    if (t.kind == Tok::LParen) {
        consume();
        Q v = parseExpr();
        expect(Tok::RParen, "')'");
        return v;
    }
    throw ParseError("Expected number, identifier or '('", t.pos);
}

}
