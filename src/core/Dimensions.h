#pragma once

#include <array>
#include <string>
#include <cstdint>

namespace pc {

// 7 base SI dimensions, in canonical order:
//   M  — mass            [kg]
//   L  — length          [m]
//   T  — time            [s]
//   I  — electric current[A]
//   Θ  — temperature     [K]
//   N  — amount          [mol]
//   J  — luminous intens.[cd]
class Dimensions {
public:
    static constexpr std::size_t kCount = 7;
    enum Axis : std::size_t { M = 0, L = 1, T = 2, I = 3, Th = 4, N = 5, J = 6 };

    using Array = std::array<int, kCount>;

    Dimensions() : exps_{0, 0, 0, 0, 0, 0, 0} {}
    explicit Dimensions(const Array& a) : exps_(a) {}

    static Dimensions Scalar()      { return Dimensions(); }
    static Dimensions Base(Axis a)  { Dimensions d; d.exps_[a] = 1; return d; }

    const Array& exps() const { return exps_; }
    int  exp(Axis a) const    { return exps_[a]; }

    bool isScalar() const;

    Dimensions& operator+=(const Dimensions& o); // multiplication of quantities
    Dimensions& operator-=(const Dimensions& o); // division
    Dimensions  operator+ (const Dimensions& o) const { auto r = *this; r += o; return r; }
    Dimensions  operator- (const Dimensions& o) const { auto r = *this; r -= o; return r; }

    Dimensions  pow(int n) const;

    bool operator==(const Dimensions& o) const { return exps_ == o.exps_; }
    bool operator!=(const Dimensions& o) const { return !(*this == o); }

    // Pretty-prints using SI base symbols: "kg*m/s^2".
    // If the dimension matches a known derived unit ("N", "J", ...), the
    // canonical symbol is preferred.
    std::string toString() const;

private:
    Array exps_;
};

}
