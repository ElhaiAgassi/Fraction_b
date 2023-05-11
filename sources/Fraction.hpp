#pragma once

#include <iostream>

#define EPSILON 0.0001f

namespace ariel
{
    enum class Operation
    {
        ADD,
        SUB,
        MUL
    };

    class Fraction
    {
    public:
        Fraction();
        Fraction(const Fraction &other);
        Fraction(int numerator, int denominator);
        Fraction(float num);

        // Arithmetic operators
        Fraction operator+(const Fraction &other) const;
        Fraction operator-(const Fraction &other) const;
        Fraction operator*(const Fraction &other) const;
        Fraction operator/(const Fraction &other) const;

        // Added overload functions for arithmetic operators with float
        friend Fraction operator+(const Fraction &fraction, float value);
        friend Fraction operator-(const Fraction &fraction, float value);
        friend Fraction operator*(const Fraction &fraction, float value);
        friend Fraction operator/(const Fraction &fraction, float value);
        // Add the other way
        friend Fraction operator+(float value, const Fraction &fraction);
        friend Fraction operator-(float value, const Fraction &fraction);
        friend Fraction operator*(float value, const Fraction &fraction);
        friend Fraction operator/(float value, const Fraction &fraction);

        // Comparison operators
        bool operator==(const Fraction &other) const;
        bool operator!=(const Fraction &other) const;
        bool operator>(const Fraction &other) const;
        bool operator<(const Fraction &other) const;
        bool operator>=(const Fraction &other) const;
        bool operator<=(const Fraction &other) const;

        // Added overload functions for comparison operators with float
        friend bool operator>(const Fraction &fraction, float value);
        friend bool operator<(const Fraction &fraction, float value);
        friend bool operator>=(const Fraction &fraction, float value);
        friend bool operator<=(const Fraction &fraction, float value);

        friend bool operator>(float value, const Fraction &fraction);
        friend bool operator<(float value, const Fraction &fraction);
        friend bool operator>=(float value, const Fraction &fraction);
        friend bool operator<=(float value, const Fraction &fraction);

        // Increment/Decrement operators
        Fraction &operator++();   // Prefix increment
        Fraction operator++(int); // Postfix increment
        Fraction &operator--();   // Prefix decrement
        Fraction operator--(int); // Postfix decrement

        // Output/Input stream operators
        friend std::ostream &operator<<(std::ostream &os, const Fraction &fraction);
        friend std::istream &operator>>(std::istream &is, Fraction &fraction);

        // Accessor functions for numerator and denominator
        int getNumerator() const;
        int getDenominator() const;

        void check_overflow(int64_t a, int64_t b, Operation op) const;
        int gcd(int a, int b) const;
        bool almostEqual(float num_a, float num_b, float epsilon = EPSILON) const;
        void reduce();

    private:
        int numerator;
        int denominator;
    };

}