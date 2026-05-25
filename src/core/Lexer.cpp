#include "Lexer.h"

#include "Exceptions.h"

#include <cctype>
#include <cstdlib>
#include <locale>
#include <sstream>

namespace pc {

Lexer::Lexer(std::string src) : src_(std::move(src)) {}

namespace {
bool isIdentStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_'; }
bool isIdentCont (char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }
}

Token Lexer::readNumber() {
    Token t;
    t.kind = Tok::Number;
    t.pos  = i_;

    std::size_t start = i_;
    while (i_ < src_.size() && std::isdigit(static_cast<unsigned char>(src_[i_]))) ++i_;
    if (i_ < src_.size() && src_[i_] == '.') {
        ++i_;
        while (i_ < src_.size() && std::isdigit(static_cast<unsigned char>(src_[i_]))) ++i_;
    }
    if (i_ < src_.size() && (src_[i_] == 'e' || src_[i_] == 'E')) {
        ++i_;
        if (i_ < src_.size() && (src_[i_] == '+' || src_[i_] == '-')) ++i_;
        if (i_ >= src_.size() || !std::isdigit(static_cast<unsigned char>(src_[i_]))) {
            throw ParseError("Malformed exponent in number", start);
        }
        while (i_ < src_.size() && std::isdigit(static_cast<unsigned char>(src_[i_]))) ++i_;
    }

    t.text = src_.substr(start, i_ - start);

    // Use a classic-locale istringstream so a comma-decimal system locale
    // (e.g. ru_RU) does not make "9.8" parse as 9.
    std::istringstream iss(t.text);
    iss.imbue(std::locale::classic());
    iss >> t.number;
    return t;
}

Token Lexer::readIdent() {
    Token t;
    t.kind = Tok::Ident;
    t.pos  = i_;
    std::size_t start = i_;
    ++i_;
    while (i_ < src_.size() && isIdentCont(src_[i_])) ++i_;
    t.text = src_.substr(start, i_ - start);
    return t;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> out;
    while (i_ < src_.size()) {
        char c = src_[i_];
        if (std::isspace(static_cast<unsigned char>(c))) { ++i_; continue; }

        if (std::isdigit(static_cast<unsigned char>(c)) ||
            (c == '.' && i_ + 1 < src_.size() &&
             std::isdigit(static_cast<unsigned char>(src_[i_ + 1])))) {
            out.push_back(readNumber());
            continue;
        }
        if (isIdentStart(c)) { out.push_back(readIdent()); continue; }

        Token t; t.pos = i_; t.text = std::string(1, c);
        switch (c) {
            case '+': t.kind = Tok::Plus;   break;
            case '-': t.kind = Tok::Minus;  break;
            case '*': t.kind = Tok::Star;   break;
            case '/': t.kind = Tok::Slash;  break;
            case '^': t.kind = Tok::Caret;  break;
            case '(': t.kind = Tok::LParen; break;
            case ')': t.kind = Tok::RParen; break;
            case '=': t.kind = Tok::Assign; break;
            default:
                throw ParseError(std::string("Unexpected character '") + c + "'", i_);
        }
        ++i_;
        out.push_back(t);
    }
    Token end; end.kind = Tok::End; end.pos = i_;
    out.push_back(end);
    return out;
}

}
