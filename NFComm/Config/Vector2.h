#ifndef __NFCOMM_CONFIG_Vector2_H__
#define __NFCOMM_CONFIG_Vector2_H__

#include <math.h>

/**
* This helper class is totally based on Ogre's Vector2 class.
*/

class Vector2
{
public:
    float x, y;

    inline Vector2() : x(0), y(0)
    {
    }

    inline Vector2(float x, float y) : x(x), y(y)
    {
    }

    inline Vector2& operator =(const Vector2& vec)
    {
        x = vec.x;
        y = vec.y;

        return *this;
    }

    inline Vector2 operator +(const Vector2& rhs) const
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }

    inline Vector2 operator -(const Vector2& rhs) const
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    inline Vector2 operator *(const float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    inline Vector2 operator /(const float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    inline Vector2& operator +=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    inline Vector2& operator -=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    inline float length()
    {
        return sqrt(x * x + y * y);
    }

    inline float distance(const Vector2& vec)
    {
        return sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y));
    }

    inline float normalise()
    {
        float length = this->length();

        if (length > 1e-08)
        {
            float invLen = 1.0f / length;
            x *= invLen;
            y *= invLen;
        }

        return length;
    }
};
#endif