#include <gamelib/vector2.h>

#include <cmath>

#include <gamelib/constants.h>

Vector2::Vector2() :
        x(0.0),
        y(0.0)
    {
        // Empty Constructor
    }

Vector2::Vector2(
    const double x,
    const double y) :
    x(x),
    y(y)
{
    // Empty Constructor
}

double Vector2::dot(const Vector2& other) const
{
    return x * other.x + y * other.y;
}

double Vector2::cross(const Vector2& other) const
{
    return x * other.y - y * other.x;
}

double Vector2::magnitude() const
{
    return std::sqrt(magnitude_squared());
}

double Vector2::magnitude_squared() const
{
    return x * x + y * y;
}

Vector2 Vector2::rotate_rad(const double angle) const
{
    const double sa = std::sin(angle);
    const double ca = std::cos(angle);
    return Vector2(
        x * ca + -y * sa,
        x * sa + y * ca);
}

Vector2 Vector2::rotate_deg(const double angle) const
{
    return rotate_rad(angle * gio::pi / 180.0);
}

Vector2 Vector2::normalize() const
{
    const double mag = magnitude();
    return Vector2(
        x / mag,
        y / mag);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(
        x + other.x,
        y + other.y);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator+=(const double val)
{
    x += val;
    y += val;
    return *this;
}

Vector2 Vector2::operator-() const
{
    return Vector2(
        -x,
        -y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(
        x - other.x,
        y - other.y);
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    *this += -other;
    return *this;
}

Vector2& Vector2::operator-=(const double val)
{
    *this += -val;
    return *this;
}

Vector2& Vector2::operator*=(const double val)
{
    x *= val;
    y *= val;
    return *this;
}

Vector2& Vector2::operator/=(const double val)
{
    x /= val;
    y /= val;
    return *this;
}

double Vector2::distance_to(const Vector2& other) const
{
    return (*this - other).magnitude();
}

Vector2 operator+(const double val, const Vector2& vec)
{
    return Vector2(
        val + vec.x,
        val + vec.y);
}

Vector2 operator+(const Vector2& vec, const double val)
{
    return Vector2(
        vec.x + val,
        vec.y + val);
}

Vector2 operator-(const double val, const Vector2& vec)
{
    return val + -vec;
}

Vector2 operator-(const Vector2& vec, const double val)
{
    return vec + -val;
}

Vector2 operator*(const Vector2& vec, const double val)
{
    return Vector2(
        vec.x * val,
        vec.y * val);
}

Vector2 operator*(const double val, const Vector2& vec)
{
    return vec * val;
}

Vector2 operator/(const Vector2& vec, const double val)
{
    return vec * (1.0 / val);
}

Vector2 operator/(const double val, const Vector2& vec)
{
    return (1.0 / val) * vec;
}
