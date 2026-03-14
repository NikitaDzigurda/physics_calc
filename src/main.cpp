#include <iostream>
#include <vector>
#include <string>



struct SizeException{
    unsigned long degree;
};

struct DimensionsException{
};


class Dimensions{

    private:
        std::vector<int> dimension;

    public:
        Dimensions(std::vector<int> d){
            if (d.size() != 3){
                throw SizeException{d.size()};
            }
            dimension = d;
        }

        Dimensions& operator+=(const Dimensions& other){
            if (dimension.size() != other.dimension.size()){
                throw SizeException{std::max(dimension.size(), other.dimension.size())};
            }

            for (size_t i = 0; i < dimension.size(); ++i){
                dimension[i] += other.dimension[i];
            }

            return *this;
        }

        Dimensions& operator-=(const Dimensions& other){
            if (dimension.size() != other.dimension.size()){
                throw SizeException{std::max(dimension.size(), other.dimension.size())};
            }

            for (size_t i = 0; i < dimension.size(); ++i){
                dimension[i] -= other.dimension[i];
            }

            return *this;
        }

        Dimensions operator+(const Dimensions& other) const {
            Dimensions result(*this);
            result += other;
            return result;
        }

        Dimensions operator-(const Dimensions& other) const {
            Dimensions result(*this);
            result -= other;
            return result;
        }

        bool operator==(const Dimensions& other) const {
            return dimension == other.dimension;
        }

        bool operator!=(const Dimensions& other) const {
            return !(*this == other);
        }

        std::vector<int> GetDegree() const{
            return dimension;
        }
};

// std::ostream& operator<<(std::ostream& os, const Dimensions& d) {
//     const auto v = d.GetDegree();
//     os << '[' << v[0] << ',' << v[1] << ',' << v[2] << ']';
//     return os;
// }

std::ostream& operator<<(std::ostream& os, const Dimensions& d) {
    std::string result;
    std::vector<std::string> si {"kg", "s", "m"};

    const auto v = d.GetDegree();
    for (size_t i=0; i < v.size(); ++i){
        if (v[i] != 0){
            if (v[i] == 1){
                result += (si[i] + '*');
            }
            else{
                result += (si[i] + '^' + std::to_string(v[i]) + '*');
            }
        }
    }
    if (result.empty()) {
        os << "1";
    } else {
        result.pop_back();
        os << result;
    }
    return os;
}

template <typename T>
class Quantity{
    private:
        T value;
        Dimensions dims;

    public:
        Quantity(const T& num, Dimensions d): value(num), dims(d) {}

        Quantity operator*(const Quantity& other) const{
            Quantity result(*this);
            result.value *= other.value;
            result.dims += other.dims;
            return result;
        }

        Quantity operator/(const Quantity& other) const{
            Quantity result(*this);
            result.value /= other.value;
            result.dims -= other.dims;
            return result;
        }

        Quantity operator*(T scalar) const{
            Quantity result(*this);
            result.value *= scalar;
            return result;
        }

        Quantity operator/(T scalar) const{
            Quantity result(*this);
            result.value /= scalar;
            return result;
        }

        Quantity& operator+=(const Quantity& other){
            if (dims != other.dims){
                throw DimensionsException{};
            }
            value += other.value;
            return *this;
        }

        Quantity& operator-=(const Quantity& other){
            if (dims != other.dims){
                throw DimensionsException{};
            }
            value -= other.value;
            return *this;
        }

        Quantity operator+(const Quantity& other) const{
            if (other.dims != dims){
                throw DimensionsException{};
            }

            Quantity result(*this);
            result.value += other.value;
            return result;
        }

        Quantity operator-(const Quantity& other) const{
            if (other.dims != dims){
                throw DimensionsException{};
            }

            Quantity result(*this);
            result.value -= other.value;
            return result;
        }

        T GetValue() const { return value; }

        const Dimensions& GetDegree() const { return dims; }
};

template <typename T>
Quantity<T> operator*(const T& scalar, const Quantity<T>& q) {
    return q * scalar;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Quantity<T>& n){
    const auto num = n.GetValue();
    const auto& d = n.GetDegree();
    os << num << " " << d;
    return os;
}

struct Base_dimens{
    Dimensions mass_base {{1, 0, 0}};
    Dimensions time_base {{0, 1, 0}};
    Dimensions length_base {{0, 0, 1}};
};

const Quantity<double> KG(1.0, Dimensions({1, 0, 0}));
const Quantity<double> SECOND(1.0, Dimensions({0, 1, 0}));
const Quantity<double> METER(1.0, Dimensions({0, 0, 1}));


int main() {
    // Проверка операций умножения-деления величин разной размерности

    // Dimensions value1({1, 0, 0});
    // Dimensions value2({0, 0, 1});
    // Dimensions nw1 = value1 + value2;
    // Dimensions nw2 = value1 - value2;
    // std::cout << nw1 << "\n";
    // std::cout << nw2 << "\n";

    // Quantity num1(10, value1);
    // Quantity num2(5, value2);
    // std::cout << num1 << "\n";
    // std::cout << num2 << "\n";

    // Quantity nw3 = num1 * num2;
    // Quantity nw4 = num1 / num2;
    // std::cout << nw3 << "\n";
    // std::cout << nw4 << "\n";

    // Проверка операций сложения-вычетания величин одинаковой и разной размерности (также с константой)

    // Base_dimens base_dimens;
    // Quantity res1(10, base_dimens.length_base);
    // Quantity res2(20, base_dimens.length_base);

    // Quantity res3(30, base_dimens.time_base);
    // Quantity res4(40, base_dimens.time_base);

    // auto totaldist1 = res1 + res2;
    // // auto totaldist2 = res1 + res3; // - поймаем ошибку поскольку разные размерности
    // auto totaldist3 = res4 - res3;
    // auto halfdist = totaldist3 / 2.0;

    // std:: cout << totaldist1 << "\n";
    // // std:: cout << totaldist2 << "\n";
    // std:: cout << totaldist3 << "\n";
    // std:: cout << halfdist << "\n";

    // Проверка вывода результата с помощью размерности из величин системы СИ 

    std:: cout << KG * METER / SECOND / SECOND << "\n"; // подсчет силы с помощью константных данных

    auto mass = 70.0 * KG;
    auto acceleration = 9.8 * METER / (SECOND * SECOND);
    auto force = mass * acceleration;

    std::cout << "Force: " << force << std::endl;
}