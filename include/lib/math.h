/*
 * an incomplete standard math library implementation for UniOS.
 * by angine04(me@angine.tech) 2025-01-09
 *
 * Note: some macros are borrowed from musl libc.
 * the original MIT license is as follows:
 */

// musl as a whole is licensed under the following standard MIT license:

// ----------------------------------------------------------------------
// Copyright © 2005-2020 Rich Felker, et al.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------

// Authors/contributors include:

// A. Wilcox
// Ada Worcester
// Alex Dowad
// Alex Suykov
// Alexander Monakov
// Andre McCurdy
// Andrew Kelley
// Anthony G. Basile
// Aric Belsito
// Arvid Picciani
// Bartosz Brachaczek
// Benjamin Peterson
// Bobby Bingham
// Boris Brezillon
// Brent Cook
// Chris Spiegel
// Clément Vasseur
// Daniel Micay
// Daniel Sabogal
// Daurnimator
// David Carlier
// David Edelsohn
// Denys Vlasenko
// Dmitry Ivanov
// Dmitry V. Levin
// Drew DeVault
// Emil Renner Berthing
// Fangrui Song
// Felix Fietkau
// Felix Janda
// Gianluca Anzolin
// Hauke Mehrtens
// He X
// Hiltjo Posthuma
// Isaac Dunham
// Jaydeep Patil
// Jens Gustedt
// Jeremy Huntwork
// Jo-Philipp Wich
// Joakim Sindholt
// John Spencer
// Julien Ramseier
// Justin Cormack
// Kaarle Ritvanen
// Khem Raj
// Kylie McClain
// Leah Neukirchen
// Luca Barbato
// Luka Perkov
// Lynn Ochs
// M Farkas-Dyck (Strake)
// Mahesh Bodapati
// Markus Wichmann
// Masanori Ogino
// Michael Clark
// Michael Forney
// Mikhail Kremnyov
// Natanael Copa
// Nicholas J. Kain
// orc
// Pascal Cuoq
// Patrick Oppenlander
// Petr Hosek
// Petr Skocik
// Pierre Carrier
// Reini Urban
// Rich Felker
// Richard Pennington
// Ryan Fairfax
// Samuel Holland
// Segev Finer
// Shiz
// sin
// Solar Designer
// Stefan Kristiansson
// Stefan O'Rear
// Szabolcs Nagy
// Timo Teräs
// Trutz Behn
// Will Dietz
// William Haddon
// William Pitcock

// Portions of this software are derived from third-party works licensed
// under terms compatible with the above MIT license:

// The TRE regular expression implementation (src/regex/reg* and
// src/regex/tre*) is Copyright © 2001-2008 Ville Laurikari and licensed
// under a 2-clause BSD license (license text in the source files). The
// included version has been heavily modified by Rich Felker in 2012, in
// the interests of size, simplicity, and namespace cleanliness.

// Much of the math library code (src/math/* and src/complex/*) is
// Copyright © 1993,2004 Sun Microsystems or
// Copyright © 2003-2011 David Schultz or
// Copyright © 2003-2009 Steven G. Kargl or
// Copyright © 2003-2009 Bruce D. Evans or
// Copyright © 2008 Stephen L. Moshier or
// Copyright © 2017-2018 Arm Limited
// and labelled as such in comments in the individual source files. All
// have been licensed under extremely permissive terms.

// The ARM memcpy code (src/string/arm/memcpy.S) is Copyright © 2008
// The Android Open Source Project and is licensed under a two-clause BSD
// license. It was taken from Bionic libc, used on Android.

// The AArch64 memcpy and memset code (src/string/aarch64/*) are
// Copyright © 1999-2019, Arm Limited.

// The implementation of DES for crypt (src/crypt/crypt_des.c) is
// Copyright © 1994 David Burren. It is licensed under a BSD license.

// The implementation of blowfish crypt (src/crypt/crypt_blowfish.c) was
// originally written by Solar Designer and placed into the public
// domain. The code also comes with a fallback permissive license for use
// in jurisdictions that may not recognize the public domain.

// The smoothsort implementation (src/stdlib/qsort.c) is Copyright © 2011
// Lynn Ochs and is licensed under an MIT-style license.

// The x86_64 port was written by Nicholas J. Kain and is licensed under
// the standard MIT terms.

// The mips and microblaze ports were originally written by Richard
// Pennington for use in the ellcc project. The original code was adapted
// by Rich Felker for build system and code conventions during upstream
// integration. It is licensed under the standard MIT terms.

// The mips64 port was contributed by Imagination Technologies and is
// licensed under the standard MIT terms.

// The powerpc port was also originally written by Richard Pennington,
// and later supplemented and integrated by John Spencer. It is licensed
// under the standard MIT terms.

// All other files which have no copyright comments are original works
// produced specifically for use as part of this library, written either
// by Rich Felker, the main author of the library, or by one or more
// contibutors listed above. Details on authorship of individual files
// can be found in the git version control history of the project. The
// omission of copyright and license comments in each file is in the
// interest of source tree size.

// In addition, permission is hereby granted for all public header files
// (include/* and arch/*/bits/*) and crt files intended to be linked into
// applications (crt/*, ldso/dlstart.c, and arch/*/crt_arch.h) to omit
// the copyright notice and permission notice otherwise required by the
// license, and to use these files without any requirement of
// attribution. These files include substantial contributions from:

// Bobby Bingham
// John Spencer
// Nicholas J. Kain
// Rich Felker
// Richard Pennington
// Stefan Kristiansson
// Szabolcs Nagy

// all of whom have explicitly granted such permission.

// This file previously contained text expressing a belief that most of
// the files covered by the above exception were sufficiently trivial not
// to be subject to copyright, resulting in confusion over whether it
// negated the permissions granted in the license. In the spirit of
// permissive licensing, and of not having licensing issues being an
// obstacle to adoption, that text has been removed.

#pragma once

#include <stddef.h> // IWYU pragma: keep
#include <compiler.h>

#define INFINITY 0x7f800000
#define NAN 0x7fc00000
#define M_PI 3.14159265358979323846

// implementaion from musl libc
static __inline unsigned __FLOAT_BITS(float __f)
{
	union {float __f; unsigned __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

// implementaion from musl libc
static __inline unsigned long long __DOUBLE_BITS(double __f)
{
	union {double __f; unsigned long long __i;} __u;
	__u.__f = __f;
	return __u.__i;
}


#define isnan(x) ( \
	sizeof(x) == sizeof(float) ? \
    (__FLOAT_BITS(x) & 0x7fffffff) > 0x7f800000 : \
    (__DOUBLE_BITS(x) & -1ULL>>1) > 0x7ffULL<<52 )

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

/*!
 * @brief Calculate the maximum of two floats
 *
 * @param a The first number
 * @param b The second number
 * @return The maximum of a and b
 */
float fmaxf(float a, float b);

/*!
 * @brief Calculate the minimum of two floats
 *
 * @param a The first number
 * @param b The second number
 * @return The minimum of a and b
 */
float fminf(float a, float b);

/*!
 * @brief Calculate the absolute value of a float
 *
 * @param x The number to calculate the absolute value of
 * @return The absolute value of x
 */
float fabsf(float x);

/*!
 * @brief Composes a floating-point value with the magnitude of x and the sign of y
 *
 * @param x The number to compose
 * @param y The number to compose with
 * @return The composed floating-point value
 */
float copysignf(float x, float y);

/*!
 * @brief Calculate the cosine of a float
 *
 * @param x The number to calculate the cosine of
 * @return The cosine of x
 */
float cos(float x);

/*!
 * @brief Calculate the modulus of a float
 *
 * @param x The number to calculate the modulus of
 * @param y The number to calculate the modulus with
 * @return The modulus of x and y
 */
float fmodf(float x, float y);
