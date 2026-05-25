#pragma once

#include "Quantity.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace pc {

// Catalogue of identifiers usable in expressions: SI base units, common
// derived units, decimal prefixes (km, mm, kJ, ...) and a handful of
// physical constants. Resolves names → Quantity<double>.
class UnitRegistry {
public:
    UnitRegistry();

    bool has(const std::string& name) const;
    Q    get(const std::string& name) const; // throws UnknownIdentifier

    // For UI: list of (symbol, description, value) entries grouped by section.
    struct Entry { std::string symbol; std::string description; Q value; };
    struct Section { std::string title; std::vector<Entry> entries; };
    const std::vector<Section>& sections() const { return sections_; }

private:
    void addUnit(const std::string& section, const std::string& sym,
                 const std::string& desc, const Q& q);

    std::unordered_map<std::string, Q> table_;
    std::vector<Section> sections_;
};

}
