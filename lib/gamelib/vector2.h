#ifndef GIO_VECTOR2_H
#define GIO_VECTOR2_H

/**
 * @brief a 2D vector library
 */
class Vector2
{
public:
    /**
     * @brief constructs a Vector2 object with (0, 0)
     */
    Vector2();

    /**
     * @brief constructs a Vector 2 with (x, y)
     * @param x is the x coordinate to use
     * @param y is the y coordinate to use
     */
    explicit Vector2(
        const double x,
        const double y);

    /**
     * @brief computes the dot product of two vectors
     * @param other the second vector operand in the dot product
     * @return the resulting dot product
     */
    double dot(const Vector2& other) const;

    /**
     * @brief computes the cross product of two vectors
     * @param other the second vector operand in the cross product
     * @return the result of this cross other
     */
    double cross(const Vector2& other) const;

    /**
     * @brief computes the vector magnitude
     * @return the vector magnitude
     */
    double magnitude() const;

    /**
     * @brief computes the vector magnitude squared
     * @return the squared vector magnitude
     */
    double magnitude_squared() const;

    /**
     * @brief rotates the vector for the given angle
     * @param angle the amount to rotate the vector by, in radians
     * @return a new rotated vector
     */
    Vector2 rotate_rad(const double angle) const;

    /**
     * @brief computes the vector norm
     * @return the normalized version of the current vector
     */
    Vector2 normalize() const;

    /**
     * @brief computes the result of adding two vectors together
     * @param other the second operand in the addition operation
     * @return a new vector resulting in the element-wise summation
     */
    Vector2 operator+(const Vector2& other) const;

    /**
     * @brief computes the result of adding two vectors together
     * @param other the second operand in the addition operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2& operator+=(const Vector2& other);

    /**
     * @brief computes the result of adding a scalar
     * @param other the second operand in the addition operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2& operator+=(const double val);

    /**
     * @brief negates the current vector
     * @return a new vector with negated values
     */
    Vector2 operator-() const;

    /**
     * @brief computes the result of subtracting two vectors together
     * @param other the second operand in the subtraction operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2 operator-(const Vector2& other) const;

    /**
     * @brief computes the result of subtracting two vectors together
     * @param other the second operand in the subtracting operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2& operator-=(const Vector2& other);

    /**
     * @brief computes the result of subtracting a scalar
     * @param other the second operand in the subtraction operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2& operator-=(const double val);

    /**
     * @brief computes the result of multiplying by a scalar
     * @param other the second operand in the multiplication operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2& operator*=(const double val);

    /**
     * @brief computes the result of dividing by a scalar
     * @param other the second operand in the division operation
     * @return a reference to the current vector with the operation completed
     */
    Vector2& operator/=(const double val);

public:
    double x;
    double y;
};

/**
 * @brief addition of a scalar and a vector
 * @param val the scalar value
 * @param vec the vector value
 * @return a new vector with the scalar added to the vector
 */
Vector2 operator+(const double val, const Vector2& vec);

/**
 * @brief addition of a vector and a scalar
 * @param vec the vector value
 * @param val the scalar value
 * @return a new vector with the scalar added to the vector
 */
Vector2 operator+(const Vector2& vec, const double val);

/**
 * @brief subtraction of a vector from a scalar
 * @param val the scalar value
 * @param vec the vector value
 * @return a new vector with the vector subtracted from the scalar
 */
Vector2 operator-(const double val, const Vector2& vec);

/**
 * @brief subtraction of a scalar from a vector
 * @param vec the vector value
 * @param val the scalar value
 * @return a new vector with thes scalar subtracted from the vector
 */
Vector2 operator-(const Vector2& vec, const double val);

/**
 * @brief multiplication of a vector and a scalar
 * @param vec the vector value
 * @param val the scalar value
 * @return a new vector with the scalar multiplied to the vector
 */
Vector2 operator*(const Vector2& vec, const double val);

/**
 * @brief multiplication of a vector and a scalar
 * @param val the scalar value
 * @param vec the vector value 
 * @return a new vector with the scalar multiplied to the vector
 */
Vector2 operator*(const double val, const Vector2& vec);

/**
 * @brief division of a vector by a scalar
 * @param vec the vector value
 * @param val the scalar value
 * @return a new vector with the vector divided by the scalar
 */
Vector2 operator/(const Vector2& vec, const double val);

/**
 * @brief division of a scalar by a vector
 * @param val the scalar value
 * @param vec the vector value
 * @return a new vector with the scalar divided by the vector
 */
Vector2 operator/(const double val, const Vector2& vec);

#endif // GIO_VECTOR2_H
