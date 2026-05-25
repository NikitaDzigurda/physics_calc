#pragma once

#include <string>
#include <vector>
#include <cstddef>

namespace pc {

enum class Tok {
    End,
    Number,
    Ident,
    Plus, Minus, Star, Slash, Caret,
    LParen, RParen,
    Assign,
};

struct Token {
    Tok         kind = Tok::End;
    std::string text;
    double      number = 0.0;
    std::size_t pos   = 0;
};

class Lexer {
public:
    explicit Lexer(std::string src);
    std::vector<Token> tokenize(); // throws ParseError
private:
    std::string src_;
    std::size_t i_ = 0;
    Token readNumber();
    Token readIdent();
};

}
