module;

#include <cmath>

#include <exception>
#include <stdexcept>

export module vector2;

export class Vector2
{
public:
    Vector2() :
        x(0.0),
        y(0.0)
    {
        // Empty Constructor
    }

    explicit Vector2(
        const double x,
        const double y) :
        x(x),
        y(y)
    {
        // Empty Constructor
    }

    double dot(const Vector2& other) const
    {
        return x * other.x + y * other.y;
    }

    double cross(const Vector2& other) const
    {
        return x * other.y - y * other.x;
    }

    double magnitude() const
    {
        return std::sqrt(magnitude_squared());
    }

    double magnitude_squared() const
    {
        return x * x + y * y;
    }

    Vector2 rotate_rad(const double angle) const
    {
        const double sa = std::sin(angle);
        const double ca = std::cos(angle);
        return Vector2(
            x * ca + y * sa,
            -y * sa + x * ca);
    }

    Vector2 normalize() const
    {
        const double mag = magnitude();
        return Vector2(
            x / mag,
            y / mag);
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(
            x + other.x,
            y + other.y);
    }

    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator+=(const double val)
    {
        x += val;
        y += val;
        return *this;
    }

    Vector2 operator-() const
    {
        return Vector2(
            -x,
            -y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(
            x - other.x,
            y - other.y);
    }

    Vector2& operator-=(const Vector2& other)
    {
        *this += -other;
        return *this;
    }

    Vector2& operator-=(const double val)
    {
        *this += -val;
        return *this;
    }

    Vector2& operator*=(const double val)
    {
        x *= val;
        y *= val;
        return *this;
    }

    Vector2& operator/=(const double val)
    {
        x /= val;
        y /= val;
        return *this;
    }

public:
    double x;
    double y;
};

export Vector2 operator+(const double val, const Vector2& vec)
{
    return Vector2(
        val + vec.x,
        val + vec.y);
}

export Vector2 operator+(const Vector2& vec, const double val)
{
    return Vector2(
        vec.x + val,
        vec.y + val);
}

export Vector2 operator-(const double val, const Vector2& vec)
{
    return val + -vec;
}

export Vector2 operator-(const Vector2& vec, const double val)
{
    return vec + -val;
}

export Vector2 operator*(const Vector2& vec, const double val)
{
    return Vector2(
        vec.x * val,
        vec.y * val);
}

export Vector2 operator*(const double val, const Vector2& vec)
{
    return vec * val;
}

export Vector2 operator/(const Vector2& vec, const double val)
{
    return vec * (1.0 / val);
}

export Vector2 operator/(const double val, const Vector2& vec)
{
    return (1.0 / val) * vec;
}
