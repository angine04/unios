#pragma once

#include <stddef.h> // IWYU pragma: keep
#include <compiler.h>

#define idiv_floor(x, ud)                          \
    ({                                             \
        auto _x  = x;                              \
        auto _ud = ud;                             \
        _x >= 0 ? _x / _ud : (_x - _ud + 1) / _ud; \
    })

#define idiv_ceil(x, ud)                          \
    ({                                            \
        auto _x  = x;                             \
        auto _ud = ud;                            \
        _x > 0 ? (_x + _ud - 1) / _ud : _x / _ud; \
    })

#define min(x, y)          \
    ({                     \
        auto _x = x;       \
        auto _y = y;       \
        _x < _y ? _x : _y; \
    })

#define max(x, y)          \
    ({                     \
        auto _x = x;       \
        auto _y = y;       \
        _x > _y ? _x : _y; \
    })

#define round_down(a, n)            \
    ({                              \
        size_t __a = (size_t)(a);   \
        (typeof(a))(__a - __a % n); \
    })

#define round_up(a, n)                                       \
    ({                                                       \
        size_t __n = (size_t)(n);                            \
        (typeof(a))(round_down((size_t)(a) + __n - 1, __n)); \
    })

#define clamp(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

/*!
 * @brief Calculate the square root of a float
 *
 * This function uses the infamous fast approximation algorithm,
 * aka FastInvsqrt, to calculate the square root of a float.
 *
 * @param x The number to calculate the square root of
 * @return The square root of x
 */
float sqrtf(float x);

/*!
 * @brief Calculate the natural logarithm of a float
 *
 * @param x The number to calculate the natural logarithm of
 * @return The natural logarithm of x
 */
float logf(float x);

/*!
 * @brief Calculate the power of a float
 *
 * @param base The base number
 * @param exp The exponent
 * @return The power of the base and exponent
 */
float powf(float base, float exp);

/*!
 * @brief Calculate the exponential of a float
 *
 * @param x The number to calculate the exponential of
 * @return The exponential of x
 */
float expf(float x);

/*!
 * @brief Calculate the floor of a float
 *
 * @param x The number to calculate the floor of
 * @return The floor of x
 */
float floorf(float x);
