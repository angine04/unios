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

/*!
 * @brief Calculate the square root of a float
 *
 * This function uses the infamous fast approximation algorithm,
 * aka FastInvsqrt, to calculate the square root of a float.
 *
 * @param x The number to calculate the square root of
 * @return The square root of x
 */
float sqrt_float(float x);
