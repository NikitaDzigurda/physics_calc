#include "Units.h"

#include "Exceptions.h"

namespace pc {

namespace {
using A = Dimensions::Axis;

Q baseUnit(A axis) {
    return Q(1.0, Dimensions::Base(axis));
}

Q scalar(double v) { return Q(v); }

}

void UnitRegistry::addUnit(const std::string& section,
                           const std::string& sym,
                           const std::string& desc,
                           const Q& q) {
    table_.emplace(sym, q);
    for (auto& s : sections_) {
        if (s.title == section) {
            s.entries.push_back({sym, desc, q});
            return;
        }
    }
    sections_.push_back({section, {{sym, desc, q}}});
}

UnitRegistry::UnitRegistry() {
    // ---- base SI ----
    const Q kg  = baseUnit(A::M);
    const Q m   = baseUnit(A::L);
    const Q s   = baseUnit(A::T);
    const Q A_  = baseUnit(A::I);
    const Q K   = baseUnit(A::Th);
    const Q mol = baseUnit(A::N);
    const Q cd  = baseUnit(A::J);

    addUnit("SI base", "kg",  "kilogram (mass)",          kg);
    addUnit("SI base", "m",   "metre (length)",           m);
    addUnit("SI base", "s",   "second (time)",            s);
    addUnit("SI base", "A",   "ampere (current)",         A_);
    addUnit("SI base", "K",   "kelvin (temperature)",     K);
    addUnit("SI base", "mol", "mole (amount)",            mol);
    addUnit("SI base", "cd",  "candela (luminous int.)",  cd);

    // ---- derived SI ----
    const Q N   = kg * m / (s * s);
    const Q J   = N * m;
    const Q W   = J / s;
    const Q Pa  = N / (m * m);
    const Q Hz  = scalar(1.0) / s;
    const Q C   = A_ * s;
    const Q V   = W / A_;
    const Q Ohm = V / A_;
    const Q F   = C / V;
    const Q Wb  = V * s;
    const Q T   = Wb / (m * m);
    const Q H   = Wb / A_;
    const Q lx  = cd / (m * m);

    addUnit("Derived", "N",   "newton (force)",            N);
    addUnit("Derived", "Pa",  "pascal (pressure)",         Pa);
    addUnit("Derived", "J",   "joule (energy)",            J);
    addUnit("Derived", "W",   "watt (power)",              W);
    addUnit("Derived", "Hz",  "hertz (frequency)",         Hz);
    addUnit("Derived", "C",   "coulomb (charge)",          C);
    addUnit("Derived", "V",   "volt (potential)",          V);
    addUnit("Derived", "Ohm", "ohm (resistance)",          Ohm);
    addUnit("Derived", "F",   "farad (capacitance)",       F);
    addUnit("Derived", "Wb",  "weber (magnetic flux)",     Wb);
    addUnit("Derived", "Tesla","tesla (magnetic field)",   T);
    addUnit("Derived", "Hn",  "henry (inductance)",        H);
    addUnit("Derived", "lx",  "lux (illuminance)",         lx);

    // ---- common multiples / submultiples ----
    addUnit("Prefixed", "km",  "kilometre",        m  * 1e3);
    addUnit("Prefixed", "cm",  "centimetre",       m  * 1e-2);
    addUnit("Prefixed", "mm",  "millimetre",       m  * 1e-3);
    addUnit("Prefixed", "g",   "gram",             kg * 1e-3);
    addUnit("Prefixed", "ton", "tonne (metric)",   kg * 1e3);
    addUnit("Prefixed", "ms",  "millisecond",      s  * 1e-3);
    addUnit("Prefixed", "us",  "microsecond",      s  * 1e-6);
    addUnit("Prefixed", "min", "minute",           s  * 60.0);
    addUnit("Prefixed", "h",   "hour",             s  * 3600.0);
    addUnit("Prefixed", "kJ",  "kilojoule",        J  * 1e3);
    addUnit("Prefixed", "kW",  "kilowatt",         W  * 1e3);
    addUnit("Prefixed", "kPa", "kilopascal",       Pa * 1e3);
    addUnit("Prefixed", "MPa", "megapascal",       Pa * 1e6);
    addUnit("Prefixed", "kN",  "kilonewton",       N  * 1e3);
    addUnit("Prefixed", "mA",  "milliampere",      A_ * 1e-3);
    addUnit("Prefixed", "mV",  "millivolt",        V  * 1e-3);
    addUnit("Prefixed", "kV",  "kilovolt",         V  * 1e3);
    addUnit("Prefixed", "kHz", "kilohertz",        Hz * 1e3);
    addUnit("Prefixed", "MHz", "megahertz",        Hz * 1e6);
    addUnit("Prefixed", "GHz", "gigahertz",        Hz * 1e9);

    // ---- physical constants ----
    addUnit("Constants", "c",   "speed of light",            scalar(299792458.0)   * m / s);
    addUnit("Constants", "g",   "standard gravity",          scalar(9.80665)       * m / (s * s));
    addUnit("Constants", "G",   "gravitational constant",    scalar(6.67430e-11)   * m.pow(3) / (kg * s.pow(2)));
    addUnit("Constants", "h",   "Planck constant",           scalar(6.62607015e-34)* J * s);
    addUnit("Constants", "hbar","reduced Planck constant",   scalar(1.054571817e-34)*J * s);
    addUnit("Constants", "k_B", "Boltzmann constant",        scalar(1.380649e-23)  * J / K);
    addUnit("Constants", "N_A", "Avogadro number",           scalar(6.02214076e23) / mol);
    addUnit("Constants", "R",   "gas constant",              scalar(8.314462618)   * J / (mol * K));
    addUnit("Constants", "e",   "elementary charge",         scalar(1.602176634e-19)* C);
    addUnit("Constants", "m_e", "electron mass",             scalar(9.1093837015e-31) * kg);
    addUnit("Constants", "m_p", "proton mass",               scalar(1.67262192369e-27)* kg);
    addUnit("Constants", "eps0","vacuum permittivity",       scalar(8.8541878128e-12) * F / m);
    addUnit("Constants", "mu0", "vacuum permeability",       scalar(1.25663706212e-6) * H / m);
    addUnit("Constants", "atm", "standard atmosphere",       scalar(101325.0)         * Pa);

    // pure-scalar mathematical constants
    addUnit("Math", "pi", "circle constant",          scalar(3.14159265358979323846));
    addUnit("Math", "tau","2*pi",                     scalar(6.28318530717958647692));
}

bool UnitRegistry::has(const std::string& name) const {
    return table_.find(name) != table_.end();
}

Q UnitRegistry::get(const std::string& name) const {
    auto it = table_.find(name);
    if (it == table_.end()) throw UnknownIdentifier(name);
    return it->second;
}

}
