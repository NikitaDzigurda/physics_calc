#include "Dimensions.h"

#include <sstream>
#include <vector>
#include <utility>

namespace pc {

bool Dimensions::isScalar() const {
    for (int e : exps_) if (e != 0) return false;
    return true;
}

Dimensions& Dimensions::operator+=(const Dimensions& o) {
    for (std::size_t i = 0; i < kCount; ++i) exps_[i] += o.exps_[i];
    return *this;
}

Dimensions& Dimensions::operator-=(const Dimensions& o) {
    for (std::size_t i = 0; i < kCount; ++i) exps_[i] -= o.exps_[i];
    return *this;
}

Dimensions Dimensions::pow(int n) const {
    Dimensions r;
    for (std::size_t i = 0; i < kCount; ++i) r.exps_[i] = exps_[i] * n;
    return r;
}

namespace {

struct Derived {
    const char* symbol;
    Dimensions::Array exps;
};

// Common derived SI units, recognised on output for nicer formatting.
const std::vector<Derived>& derivedTable() {
    static const std::vector<Derived> t = {
        // symbol, [M, L, T, I, Θ, N, J]
        {"N",   { 1,  1, -2, 0, 0, 0, 0}}, // newton
        {"Pa",  { 1, -1, -2, 0, 0, 0, 0}}, // pascal
        {"J",   { 1,  2, -2, 0, 0, 0, 0}}, // joule
        {"W",   { 1,  2, -3, 0, 0, 0, 0}}, // watt
        {"Hz",  { 0,  0, -1, 0, 0, 0, 0}}, // hertz
        {"C",   { 0,  0,  1, 1, 0, 0, 0}}, // coulomb
        {"V",   { 1,  2, -3,-1, 0, 0, 0}}, // volt
        {"Ohm", { 1,  2, -3,-2, 0, 0, 0}}, // ohm
        {"S",   {-1, -2,  3, 2, 0, 0, 0}}, // siemens
        {"F",   {-1, -2,  4, 2, 0, 0, 0}}, // farad
        {"Wb",  { 1,  2, -2,-1, 0, 0, 0}}, // weber
        {"T",   { 1,  0, -2,-1, 0, 0, 0}}, // tesla
        {"H",   { 1,  2, -2,-2, 0, 0, 0}}, // henry
        {"lx",  { 0, -2,  0, 0, 0, 0, 1}}, // lux
    };
    return t;
}

void appendUnit(std::ostringstream& num,
                std::ostringstream& den,
                int& numCount, int& denCount,
                const std::string& sym, int e) {
    if (e == 0) return;
    auto write = [&](std::ostringstream& s, int& cnt, int absExp) {
        if (cnt > 0) s << '*';
        s << sym;
        if (absExp != 1) s << '^' << absExp;
        ++cnt;
    };
    if (e > 0) write(num, numCount, e);
    else       write(den, denCount, -e);
}

}

std::string Dimensions::toString() const {
    if (isScalar()) return "1";

    // Prefer a single derived-unit symbol if exactly matched.
    for (const auto& d : derivedTable()) {
        if (exps_ == d.exps) return d.symbol;
    }

    static const char* base[kCount] = {"kg", "m", "s", "A", "K", "mol", "cd"};

    std::ostringstream num, den;
    int nc = 0, dc = 0;
    for (std::size_t i = 0; i < kCount; ++i) {
        appendUnit(num, den, nc, dc, base[i], exps_[i]);
    }

    std::string n = num.str();
    std::string d = den.str();
    if (n.empty()) n = "1";
    if (d.empty()) return n;
    if (dc > 1)    return n + "/(" + d + ")";
    return n + "/" + d;
}

}
