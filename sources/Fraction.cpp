#include "Fraction.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <climits>

using namespace std;

namespace ariel
{

    // Default constructor
    Fraction::Fraction() : numerator(0), denominator(1) {}

    // Copy constructor
    Fraction::Fraction(const Fraction &other)
    {
        if (other.denominator == 0)
        {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        if (other.denominator > INT_MAX || other.numerator > INT_MAX)
        {
            throw std::overflow_error("Numerator or denominator is too large");
        }

        if (other.denominator < 0)
        {
            numerator = -other.numerator;
            denominator = -other.denominator;
        }

        this->numerator = other.numerator;
        this->denominator = other.denominator;

        this->reduce();
    }

    // Constructor with numerator and denominator
    Fraction::Fraction(int numerator, int denominator)
    {
        if (denominator == 0)
        {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        if (denominator > INT_MAX || numerator > INT_MAX)
        {
            throw std::overflow_error("Numerator or denominator is too large");
        }

        this->numerator = numerator;
        this->denominator = denominator;

        this->reduce();
    }

    // Constructor with Float
    Fraction::Fraction(float num)
    {
        double integral, fractional;
        fractional = modf(num, &integral);
        const double precision = 0.001;

        // Check for overflow
        if (integral > INT_MAX || fractional / precision > INT_MAX)
        {
            throw std::overflow_error("Number is too large");
        }

        denominator = static_cast<int>(round(1 / precision));
        numerator = static_cast<int>(round(fractional * denominator) + integral * denominator);
        reduce();
    }

    // Arithmetic operators

    Fraction Fraction::operator+(const Fraction &other) const
    {
        int64_t num = static_cast<int64_t>(numerator) * other.denominator + static_cast<int64_t>(other.numerator) * denominator;
        int64_t den = static_cast<int64_t>(denominator) * other.denominator;

        check_overflow(num, den, Operation::ADD);

        Fraction f(static_cast<int>(num), static_cast<int>(den));
        f.reduce();

        return f;
    }

    Fraction Fraction::operator-(const Fraction &other) const
    {
        check_overflow(static_cast<int64_t>(numerator) * other.denominator, static_cast<int64_t>(other.numerator) * denominator, Operation::SUB);
        int num = numerator * other.denominator - other.numerator * denominator;
        int den = denominator * other.denominator;
        Fraction f(num, den);
        f.reduce();
        return f;
    }

    Fraction Fraction::operator*(const Fraction &other) const
    {
        check_overflow(numerator, other.numerator, Operation::MUL);
        check_overflow(denominator, other.denominator, Operation::MUL);

        int newNumerator = numerator * other.numerator;
        int newDenominator = denominator * other.denominator;

        Fraction f(newNumerator, newDenominator);
        f.reduce();
        return f;
    }

    Fraction Fraction::operator/(const Fraction &other) const
    {
        if (other.numerator == 0 || other.denominator == 0)
        {
            throw std::runtime_error("Division by zero");
        }

        check_overflow(numerator, other.denominator, Operation::MUL);
        check_overflow(denominator, other.numerator, Operation::MUL);
        Fraction f(numerator * other.denominator, denominator * other.numerator);
        f.reduce();
        return f;
    }
    // Overloaded operator+ with Fraction and float operand
    Fraction operator+(const Fraction &fraction, float value)
    {
        Fraction result = fraction + Fraction(value);
        result.reduce();
        return result;
    }

    // Overloaded operator- with Fraction and float operand
    Fraction operator-(const Fraction &fraction, float value)
    {
        Fraction result = fraction - Fraction(value);
        result.reduce();
        return result;
    }

    // Overloaded operator* with Fraction and float operand
    Fraction operator*(const Fraction &fraction, float value)
    {
        Fraction result = fraction * Fraction(value);
        result.reduce();
        return result;
    }

    // Overloaded operator/ with Fraction and float operand
    Fraction operator/(const Fraction &fraction, float value)
    {
        if (value == 0)
        {
            throw std::runtime_error("Denominator cannot be zero");
        }
        Fraction result = fraction / Fraction(value);
        result.reduce();
        return result;
    }

    // Overloaded operator+ with float and Fraction operand
    Fraction operator+(float value, const Fraction &fraction)
    {
        Fraction result = fraction + value;
        result.reduce();
        return result;
    }

    // Overloaded operator- with float and Fraction operand
    Fraction operator-(float value, const Fraction &fraction)
    {
        Fraction result = Fraction(value) - fraction;
        result.reduce();
        return result;
    }

    // Overloaded operator* with float and Fraction operand
    Fraction operator*(float value, const Fraction &fraction)
    {
        Fraction result = fraction * value;
        result.reduce();
        return result;
    }

    // Overloaded operator/ with float and Fraction operand
    Fraction operator/(float value, const Fraction &fraction)
    {
        Fraction result = Fraction(value) / fraction;
        result.reduce();
        return result;
    }

    // Comparison operators

    bool Fraction::operator==(const Fraction &other_) const
    {
        Fraction that = *this;
        Fraction other = other_;
        that.reduce();
        other.reduce();
        if (std::is_floating_point<decltype(that.numerator)>::value ||
            std::is_floating_point<decltype(other.numerator)>::value)
        {
            return almostEqual(static_cast<float>(that.numerator) / that.denominator,
                               static_cast<float>(other.numerator) / other.denominator);
        }
        return (that.numerator == other.numerator) && (that.denominator == other.denominator);
    }

    bool Fraction::operator!=(const Fraction &other) const
    {
        return !(*this == other);
    }

    bool Fraction::operator>(const Fraction &other) const
    {
        return (numerator * other.denominator) > (other.numerator * denominator);
    }

    bool Fraction::operator<(const Fraction &other) const
    {
        return (numerator * other.denominator) < (other.numerator * denominator);
    }

    bool operator==(const Fraction &fraction, float value)
    {
        return fraction == Fraction(value);
    }

    bool operator!=(const Fraction &fraction, float value)
    {
        return !(fraction == Fraction(value));
    }

    bool operator==(float value, const Fraction &fraction)
    {
        return fraction == Fraction(value);
    }

    bool operator!=(float value, const Fraction &fraction)
    {
        return fraction != Fraction(value);
    }

    // Overloaded operator> with float and Fraction operand
    bool operator>(const Fraction &fraction, float value)
    {
        return fraction > Fraction(value);
    }

    // Overloaded operator< with Fraction and float operand
    bool operator<(const Fraction &fraction, float value)
    {
        return fraction < Fraction(value);
    }

    // Overloaded operator>= with Fraction and float operand
    bool operator>=(const Fraction &fraction, float value)
    {
        return fraction >= Fraction(value);
    }

    // Overloaded operator<= with Fraction and float operand
    bool operator<=(const Fraction &fraction, float value)
    {
        return fraction <= Fraction(value);
    }

    bool Fraction::operator>=(const Fraction &other) const
    {
        return !(*this < other);
    }

    bool Fraction::operator<=(const Fraction &other) const
    {
        return !(*this > other);
    }

    // Overloaded operators for comparisons with float as the left operand

    bool operator>(float value, const Fraction &fraction)
    {
        return Fraction(value) > fraction;
    }

    bool operator<(float value, const Fraction &fraction)
    {
        return Fraction(value) < fraction;
    }

    bool operator>=(float value, const Fraction &fraction)
    {
        return Fraction(value) >= fraction;
    }

    bool operator<=(float value, const Fraction &fraction)
    {
        return Fraction(value) <= fraction;
    }

    // Increment/Decrement operators

    Fraction &Fraction::operator++()
    {
        numerator += denominator;
        this->reduce();
        return *this;
    }

    Fraction Fraction::operator++(int)
    {
        Fraction temp(*this);
        numerator += denominator;
        this->reduce();
        return temp;
    }
    Fraction &Fraction::operator--()
    {
        numerator -= denominator;
        this->reduce();
        return *this;
    }

    Fraction Fraction::operator--(int)
    {
        Fraction temp(*this);
        numerator -= denominator;
        this->reduce();
        return temp;
    }

    // Output/Input stream operators

    std::ostream &operator<<(std::ostream &os, const Fraction &fraction_)
    {
        Fraction fraction;
        if (fraction_.denominator < 0)
        {
            fraction = Fraction(-fraction_.getNumerator(), -fraction_.getDenominator());
        }
        else
        {
            fraction = fraction_;
        }

        os << fraction.numerator << "/" << fraction.denominator;
        return os;
    }

    istream &operator>>(istream &is, Fraction &fraction)
    {
        if (!(is >> fraction.numerator >> fraction.denominator))
        {
            throw std::runtime_error("Invalid input format");
        }
        if (fraction.denominator == 0)
        {
            throw std::runtime_error("Denominator cannot be zero");
        }
        fraction.reduce();
        return is;
    }

    // Accessor functions for numerator and denominator
    int Fraction::getNumerator() const
    {
        return numerator;
    }

    int Fraction::getDenominator() const
    {
        return denominator;
    }

    int Fraction::gcd(int a, int b) const
    {
        if (b == 0)
        {
            return a;
        }
        return gcd(b, a % b);
    }

    void Fraction::check_overflow(int64_t a, int64_t b, Operation op) const
    {
        switch (op)
        {
        case Operation::ADD:
            if (a > INT_MAX || a < INT_MIN)
            {
                throw std::overflow_error("Overflow in addition operation");
            }
            break;
        case Operation::SUB:
            if (a > INT_MAX + b || a < INT_MIN - b)
            {
                throw std::overflow_error("Overflow in subtraction operation");
            }
            break;
        case Operation::MUL:
            if ((a > 0 && b > 0 && a > INT_MAX / b) || (a > 0 && b < 0 && a > INT_MIN / b) || (a < 0 && b > 0 && a < INT_MIN / b) || (a < 0 && b < 0 && a < INT_MAX / b))
            {
                throw std::overflow_error("Overflow in multiplication operation");
            }
            break;
        default:
            throw std::invalid_argument("Invalid operation");
        }
    }

    bool almostEqual(float a, float b, float epsilon = 0.0001f)
    {
        return std::abs(a - b) <= epsilon;
    }

    void Fraction::reduce()
    {
        int gcd = std::abs(Fraction::gcd(numerator, denominator));
        numerator /= gcd;
        denominator /= gcd;

        if (denominator < 0)
        {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

} // namespace ariel
