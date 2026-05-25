#pragma once

#include "Dimensions.h"
#include "Exceptions.h"

#include <cmath>
#include <locale>
#include <sstream>
#include <string>
#include <type_traits>

namespace pc {

// A physical quantity = numeric value tagged with a Dimensions vector.
// Templated on numeric T (typically double).
template <typename T>
class Quantity {
public:
    Quantity() : value_(T{}), dims_() {}
    Quantity(T v, Dimensions d) : value_(v), dims_(d) {}
    explicit Quantity(T v) : value_(v), dims_() {} // pure scalar

    T value() const { return value_; }
    const Dimensions& dims() const { return dims_; }

    // ---- arithmetic ----
    Quantity operator+(const Quantity& o) const {
        requireSameDims(o, "+");
        return Quantity(value_ + o.value_, dims_);
    }
    Quantity operator-(const Quantity& o) const {
        requireSameDims(o, "-");
        return Quantity(value_ - o.value_, dims_);
    }
    Quantity operator*(const Quantity& o) const {
        return Quantity(value_ * o.value_, dims_ + o.dims_);
    }
    Quantity operator/(const Quantity& o) const {
        if (o.value_ == T{}) throw EvalError("Division by zero");
        return Quantity(value_ / o.value_, dims_ - o.dims_);
    }
    Quantity operator-() const { return Quantity(-value_, dims_); }
    Quantity operator+() const { return *this; }

    // scalar * Quantity helpers (convenience for building unit catalogue)
    Quantity operator*(T s) const { return Quantity(value_ * s, dims_); }
    Quantity operator/(T s) const {
        if (s == T{}) throw EvalError("Division by zero");
        return Quantity(value_ / s, dims_);
    }
    friend Quantity operator*(T s, const Quantity& q) { return q * s; }
    friend Quantity operator/(T s, const Quantity& q) {
        return Quantity(s) / q;
    }

    // Integer power — keeps dimensions integral.
    Quantity pow(int n) const {
        T v = T{1};
        T base = value_;
        int e = n < 0 ? -n : n;
        // fast exponentiation
        while (e > 0) {
            if (e & 1) v *= base;
            base *= base;
            e >>= 1;
        }
        if (n < 0) {
            if (v == T{}) throw EvalError("Division by zero in power");
            v = T{1} / v;
        }
        return Quantity(v, dims_.pow(n));
    }

    bool operator==(const Quantity& o) const {
        return dims_ == o.dims_ && value_ == o.value_;
    }
    bool operator!=(const Quantity& o) const { return !(*this == o); }

    std::string toString() const {
        std::ostringstream os;
        os.imbue(std::locale::classic());
        os.precision(10);
        os << value_;
        if (!dims_.isScalar()) os << ' ' << dims_.toString();
        return os.str();
    }

private:
    void requireSameDims(const Quantity& o, const char* op) const {
        if (dims_ != o.dims_) {
            throw DimensionMismatch(
                std::string("Incompatible dimensions for '") + op + "': " +
                dims_.toString() + " vs " + o.dims_.toString());
        }
    }

    T          value_;
    Dimensions dims_;
};

using Q = Quantity<double>;

}
