#include "Calculator.h"

#include "Exceptions.h"
#include "Lexer.h"
#include "Parser.h"

namespace pc {

Calculator::Calculator() = default;

Q Calculator::lookup(const std::string& name) const {
    auto it = vars_.find(name);
    if (it != vars_.end()) return it->second;
    return units_.get(name); // throws UnknownIdentifier on miss
}

void Calculator::assign(const std::string& name, const Q& value) {
    vars_[name] = value;
}

Calculator::HistoryEntry Calculator::evaluate(const std::string& input) {
    HistoryEntry entry;
    entry.input = input;
    try {
        Lexer lex(input);
        auto tokens = lex.tokenize();
        Parser parser(std::move(tokens), *this);
        auto result = parser.parse();
        if (result.wasAssignment) {
            entry.output = result.assignedName + " = " + result.value.toString();
            if (units_.has(result.assignedName)) {
                entry.note = "Note: '" + result.assignedName +
                             "' shadows a built-in unit (e.g., " +
                             units_.get(result.assignedName).toString() + ")";
            }
        } else {
            entry.output = result.value.toString();
        }
    } catch (const CalcError& e) {
        entry.isError = true;
        entry.output  = e.what();
    } catch (const std::exception& e) {
        entry.isError = true;
        entry.output  = std::string("Internal error: ") + e.what();
    }
    history_.push_back(entry);
    return entry;
}

}
