#pragma once

#include "Quantity.h"
#include "Units.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace pc {

// Evaluates user-entered expressions, maintains a variable environment, and
// keeps a history of evaluations for the UI.
class Calculator {
public:
    Calculator();

    struct HistoryEntry {
        std::string input;
        std::string output;   // pretty result string, or error message
        std::string note;     // optional non-fatal warning (e.g. shadowing)
        bool        isError = false;
    };

    // Evaluate one statement. Pushes a history entry. Returns the same entry
    // for convenience.
    HistoryEntry evaluate(const std::string& input);

    // Resolve a name to its value. Looks in variables first, then in the
    // unit registry. Throws UnknownIdentifier.
    Q lookup(const std::string& name) const;

    // Bind a variable.
    void assign(const std::string& name, const Q& value);

    const UnitRegistry& units() const { return units_; }
    const std::unordered_map<std::string, Q>& variables() const { return vars_; }
    const std::vector<HistoryEntry>& history() const { return history_; }

    void clearVariables() { vars_.clear(); }
    void clearHistory()   { history_.clear(); }

private:
    UnitRegistry                       units_;
    std::unordered_map<std::string, Q> vars_;
    std::vector<HistoryEntry>          history_;
};

}
