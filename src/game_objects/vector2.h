#pragma once

#include <cmath>

#include <exception>
#include <stdexcept>

class Vector2
{
public:
    Vector2();

    explicit Vector2(
        const double x,
        const double y);

    double dot(const Vector2& other) const;

    double cross(const Vector2& other) const;

    double magnitude() const;

    double magnitude_squared() const;

    Vector2 rotate_rad(const double angle) const;

    Vector2 normalize() const;

    Vector2 operator+(const Vector2& other) const;

    Vector2& operator+=(const Vector2& other);

    Vector2& operator+=(const double val);

    Vector2 operator-() const;

    Vector2 operator-(const Vector2& other) const;

    Vector2& operator-=(const Vector2& other);

    Vector2& operator-=(const double val);

    Vector2& operator*=(const double val);

    Vector2& operator/=(const double val);

public:
    double x;
    double y;
};

Vector2 operator+(const double val, const Vector2& vec);

Vector2 operator+(const Vector2& vec, const double val);

Vector2 operator-(const double val, const Vector2& vec);

Vector2 operator-(const Vector2& vec, const double val);

Vector2 operator*(const Vector2& vec, const double val);

Vector2 operator*(const double val, const Vector2& vec);

Vector2 operator/(const Vector2& vec, const double val);

Vector2 operator/(const double val, const Vector2& vec);
