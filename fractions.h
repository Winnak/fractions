/**
 * MIT LICENSE
 *
 * Copyright 2018 Erik Høyrup Jørgensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef FRACTIONS_LIBRARY_H
#define FRACTIONS_LIBRARY_H

#include <cstdint>
#include <cmath>
#include <string>
#include <sstream>

template <typename T, typename U>
struct Fraction {
    T numerator = 0;
    U denominator = 0;

    float_t float32() const {
        return float_t(numerator) / float_t(denominator);
    }

    double_t float64() const {
        return double_t(numerator) / double_t(denominator);
    }

    void reduce() {
        T r = gcd(numerator, denominator);
        numerator = numerator/r;
        denominator = denominator/r;
    }

    static Fraction<T, U> reduce(Fraction<T, U> frac) {
        T r = gcd(frac.numerator, frac.denominator);
        return {
            frac.numerator/r, frac.denominator/r
        };
    }

    static Fraction<T, U> from_float(float val) {
        float int_part;
        float dec_part = modf(val , &int_part);

        float denominator = floor(dec_part);
        int32_t exp = 1;

        while (dec_part > denominator / exp) {
            exp *= 10;
            denominator = floor(dec_part * exp);
        }

        return reduce(Fraction<T, U>{ T(int_part * exp + denominator), U(exp) });
    }

    static Fraction<T, U> from_float(double val) {
        double int_part;
        double dec_part = modf(val , &int_part);

        double denominator = floor(dec_part);
        int32_t exp = 1;

        while (dec_part > denominator / exp) {
            exp *= 10;
            denominator = floor(dec_part * exp);
        }

        return reduce(Fraction<T, U>{ T(int_part * exp + denominator), U(exp) });
    }

    inline static Fraction<T, U> add(const Fraction<T, U> &lhs, const Fraction<T, U> &rhs) {
        return {
                (lhs.numerator * rhs.denominator) + (rhs.numerator * lhs.denominator),
                lhs.denominator * rhs.denominator
        };
    }

    inline static Fraction<T, U> sub(const Fraction<T, U> &lhs, const Fraction<T, U> &rhs) {
        return {
                (lhs.numerator * rhs.denominator) - (rhs.numerator * lhs.denominator),
                lhs.denominator * rhs.denominator
        };
    }

    inline static Fraction<T, U> mul(const Fraction<T, U> &lhs, const Fraction<T, U> &rhs) {
        return {
                lhs.numerator * rhs.numerator,
                lhs.denominator * rhs.denominator
        };
    }

    inline static Fraction<T, U> div(const Fraction<T, U> &lhs, const Fraction<T, U> &rhs) {
        return {
                lhs.numerator * rhs.denominator,
                lhs.denominator * rhs.numerator
        };
    }

    inline Fraction<T, U> operator-() const {
        return { this->numerator, -this->denominator };
    }

    inline Fraction<T, U> operator--() {
        return { this->numerator - 1 * this->denominator, this->denominator };
    }

    inline Fraction<T, U> operator++() {
        return { this->numerator + 1 * this->denominator, this->denominator };
    }

    static T cmp(const Fraction<T, U> &lhs, const Fraction<T, U> &rhs) {
        if (lhs.numerator == 0 || rhs.numerator == 0) {
            return lhs.numerator - rhs.numerator;
        }

        auto reduced_lhs = reduce(lhs);
        auto reduced_rhs = reduce(rhs);
        return sub(reduced_lhs, reduced_rhs).numerator;
    }

    Fraction<T, U> copy() const {
        return Fraction{ this->numerator, this->denominator };
    };

    std::string to_string() const {
        std::ostringstream stringStream;
        stringStream << numerator << "/" << denominator;
        return stringStream.str();
    }

private:
    static T gcd(T a, T b) {
        if (a < 0) {
            a = -a;
        }

        if (b < 0) {
            b = -b;
        }


        while (b != 0) {
            a %= b;
            if (a == 0) {
                return b;
            }

            b %= a;
        }

        return a;
    }

};

// Arithmetic
template <typename T, typename U> inline Fraction<T, U> operator+(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs) { return Fraction<T, U>::add(lhs, rhs); }
template <typename T, typename U> inline Fraction<T, U> operator+(const Fraction<T, U>& lhs, const U rhs) { return { lhs.numerator + rhs * lhs.denominator, lhs.denominator }; }
template <typename T, typename U> inline Fraction<T, U> operator+(const U rhs, const Fraction<T, U>& lhs) { return { lhs.numerator + rhs * lhs.denominator, lhs.denominator }; }

template <typename T, typename U> inline Fraction<T, U> operator-(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs) { return Fraction<T, U>::sub(lhs, rhs); }
template <typename T, typename U> inline Fraction<T, U> operator-(const Fraction<T, U>& lhs, const U rhs) { return { lhs.numerator - rhs * lhs.denominator, lhs.denominator }; }
template <typename T, typename U> inline Fraction<T, U> operator-(const U rhs, const Fraction<T, U>& lhs) { return { lhs.numerator - rhs * lhs.denominator, lhs.denominator }; }

template <typename T, typename U> inline Fraction<T, U> operator*(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs) { return Fraction<T, U>::mul(lhs, rhs); }
template <typename T, typename U> inline Fraction<T, U> operator*(const Fraction<T, U>& lhs, const U rhs) { return { lhs.numerator * rhs, lhs.denominator }; }
template <typename T, typename U> inline Fraction<T, U> operator*(const U rhs, const Fraction<T, U>& lhs) { return { lhs.numerator * rhs, lhs.denominator }; }

template <typename T, typename U> inline Fraction<T, U> operator/(const Fraction<T, U>& lhs, const U rhs) { return { lhs.numerator, lhs.denominator * rhs }; }
template <typename T, typename U> inline Fraction<T, U> operator/(const U rhs, const Fraction<T, U>& lhs) { return { lhs.numerator, lhs.denominator * rhs }; }
template <typename T, typename U> inline Fraction<T, U> operator/(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs) { return Fraction<T, U>::div(lhs, rhs); }

// Comparisons
template <typename T, typename U> inline bool operator==(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs){ return Fraction<T, U>::cmp(lhs, rhs) == 0; }
template <typename T, typename U> inline bool operator!=(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs){ return Fraction<T, U>::cmp(lhs, rhs) != 0; }
template <typename T, typename U> inline bool operator< (const Fraction<T, U>& lhs, const Fraction<T, U>& rhs){ return Fraction<T, U>::cmp(lhs, rhs) <  0; }
template <typename T, typename U> inline bool operator> (const Fraction<T, U>& lhs, const Fraction<T, U>& rhs){ return Fraction<T, U>::cmp(lhs, rhs) >  0; }
template <typename T, typename U> inline bool operator<=(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs){ return Fraction<T, U>::cmp(lhs, rhs) <= 0; }
template <typename T, typename U> inline bool operator>=(const Fraction<T, U>& lhs, const Fraction<T, U>& rhs){ return Fraction<T, U>::cmp(lhs, rhs) >= 0; }


template <typename T, typename U> inline Fraction<T, U> operator+=(Fraction<T, U>& self, const Fraction<T, U>& other) {
    auto tmp = Fraction<T, U>::add(self, other);
    self.numerator = tmp.numerator;
    self.denominator = tmp.denominator;
    return self;
}

template <typename T, typename U> inline Fraction<T, U> operator-=(Fraction<T, U>& self, const Fraction<T, U>& other) {
    auto tmp = Fraction<T, U>::sub(self, other);
    self.numerator = tmp.numerator;
    self.denominator = tmp.denominator;
    return self;
}

template <typename T, typename U> inline Fraction<T, U> operator*=(Fraction<T, U>& self, const Fraction<T, U>& other) {
    auto tmp = Fraction<T, U>::mul(self, other);
    self.numerator = tmp.numerator;
    self.denominator = tmp.denominator;
    return self;
}

template <typename T, typename U> inline Fraction<T, U> operator/=(Fraction<T, U>& self, const Fraction<T, U>& other) {
    auto tmp = Fraction<T, U>::div(self, other);
    self.numerator = tmp.numerator;
    self.denominator = tmp.denominator;
    return self;
}

typedef Fraction<int8_t, int8_t> frac8;
typedef Fraction<int16_t, int16_t> frac16;
typedef Fraction<int32_t, int32_t> frac32;
typedef Fraction<int64_t, int64_t> frac64;

#endif