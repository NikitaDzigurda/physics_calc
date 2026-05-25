#pragma once

#include <stdexcept>
#include <string>

namespace pc {

class CalcError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class DimensionMismatch : public CalcError {
public:
    DimensionMismatch(const std::string& msg) : CalcError(msg) {}
};

class ParseError : public CalcError {
public:
    ParseError(const std::string& msg, std::size_t pos = 0)
        : CalcError(msg), position(pos) {}
    std::size_t position;
};

class EvalError : public CalcError {
public:
    using CalcError::CalcError;
};

class UnknownIdentifier : public EvalError {
public:
    UnknownIdentifier(const std::string& name)
        : EvalError("Unknown identifier: '" + name + "'"), identifier(name) {}
    std::string identifier;
};

}
