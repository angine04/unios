/*
 * an incomplete standard math library implementation for UniOS.
 * by angine04(me@angine.tech) 2025-01-09
 *
 * Note: some functions are borrowed from musl libc.
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

#include <math.h>

// Fast exp implementation
float expf(float x) {
    volatile union {
        float        f;
        unsigned int i;
    } cvt;

    /* exp(x) = 2^i * 2^f; i = floor (log2(e) * x), 0 <= f <= 1 */
    float t  = x * 1.442695041f;
    float fi = floorf(t);
    float f  = t - fi;
    int   i  = (int)fi;
    cvt.f =
        (0.3371894346f * f + 0.657636276f) * f + 1.00172476f; /* compute 2^f */
    cvt.i += (i << 23);                                       /* scale by 2^i */
    return cvt.f;
}

float sqrtf(float x) {
    // the infamous fast inverse square root algorithm
    float xhalf = 0.5f * x;
    int   i     = *(int *)&x; // evil floating point bit level hacking
    i           = 0x5f3759df - (i >> 1); // what the fuck?
    x           = *(float *)&i;
    x = x * (1.5f - xhalf * x * x); // one iteration of Newton-Raphson is enough
    return 1.0f / x;
}

// Compute natural log using Taylor series
float logf(float x) {
    if (x <= 0) return 0.0f; // Handle invalid input

    // Use log(1+y) series where y = (x-1)/(x+1)
    float y    = (x - 1.0f) / (x + 1.0f);
    float y2   = y * y;
    float sum  = y;
    float term = y;

    // Unroll loop for better performance
    term  = term * y2;
    sum  += term / 3.0f;
    term  = term * y2;
    sum  += term / 5.0f;
    term  = term * y2;
    sum  += term / 7.0f;
    term  = term * y2;
    sum  += term / 9.0f;
    term  = term * y2;
    sum  += term / 11.0f;
    term  = term * y2;
    sum  += term / 13.0f;
    term  = term * y2;
    sum  += term / 15.0f;
    term  = term * y2;
    sum  += term / 17.0f;
    term  = term * y2;
    sum  += term / 19.0f;
    term  = term * y2;
    sum  += term / 21.0f;
    term  = term * y2;
    sum  += term / 23.0f;
    term  = term * y2;
    sum  += term / 25.0f;
    term  = term * y2;
    sum  += term / 27.0f;
    term  = term * y2;
    sum  += term / 29.0f;
    term  = term * y2;
    sum  += term / 31.0f;
    term  = term * y2;
    sum  += term / 33.0f;
    term  = term * y2;
    sum  += term / 35.0f;
    term  = term * y2;
    sum  += term / 37.0f;
    term  = term * y2;


    return 2.0f * sum;
}

// Compute x^y as e^(y*ln(x))
float powf(float x, float y) {
    // Handle special cases
    if (x == 0.0f) return (y > 0) ? 0.0f : 1.0f;
    if (y == 0.0f) return 1.0f;
    if (y == 1.0f) return x;
    if (y == 2.0f) return x * x;
    if (y == 3.0f) return x * x * x;
    if (y == -1.0f) return 1.0f / x;
    if (y == 0.5f) return sqrtf(x);

    // For negative x, only allow integer exponents
    if (x < 0) {
        int yi = (int)y;
        if (y != yi) return 0.0f; // Non-integer power of negative number

        float p = powf(-x, y);
        return (yi % 2 == 0) ? p : -p;
    }

    // For very large or small values, handle specially
    if (x > 1e6f || x < 1e-6f || y > 1e3f || y < -1e3f) {
        float lnx  = logf(x);
        float ylnx = y * lnx;
        if (ylnx > 88.0f) return 1e38f;   // Avoid overflow
        if (ylnx < -88.0f) return 1e-38f; // Avoid underflow
    }

    return expf(y * logf(x));
}

float floorf(float x) {
    // Handle special cases
    if (x == 0.0f || x != x) return x; // Return x if zero or NaN

    // Get integer part
    int i = (int)x;

    // If x is negative and has a fractional part, round down
    if (x < 0.0f && x != i) { return (float)(i - 1); }

    return (float)i;
}

float fmaxf(float x, float y)
{
	return x < y ? y : x;
}


float fminf(float x, float y)
{
	return x < y ? x : y;
}

// implementation from musl libc
float fabsf(float x)
{
	union {float f; uint32_t i;} u = {x};
	u.i &= 0x7fffffff;
	return u.f;
}

// implementation from musl libc
float copysignf(float x, float y)
{
	union {float f; uint32_t i;} ux={x}, uy={y};
	ux.i &= 0x7fffffff;
	ux.i |= uy.i & 0x80000000;
	return ux.f;
}



float cos(float x) {
    // Reduce range to [0, 2π]
    x = fmodf(x, 2.0f * M_PI);
    if (x < 0) x += 2.0f * M_PI;

    // Get quadrant (0 to 3)
    float quarter = x / (M_PI/2.0f);
    int quadrant = (int)quarter;

    // Reduce to [0, π/2]
    x = x - quadrant * (M_PI/2.0f);

    // Adjust x based on quadrant
    switch(quadrant % 4) {
        case 1: x = M_PI/2.0f - x; break;  // [π/2, π]
        case 2: x = -x; break;             // [π, 3π/2]
        case 3: x = -(M_PI/2.0f - x); break; // [3π/2, 2π]
    }

    float c0 = 0.9999932946f;
    float c2 = -0.4999124376f;
    float c4 = 0.0414877472f;
    float c6 = -0.0012712095f;

    float x2 = x * x;
    float result = c6;
    result = result * x2 + c4;
    result = result * x2 + c2;
    result = result * x2 + c0;

    return (quadrant == 1 || quadrant == 2) ? -result : result;
}

// implementation from musl libc
float fmodf(float x, float y)
{
	union {float f; uint32_t i;} ux = {x}, uy = {y};
	int ex = ux.i>>23 & 0xff;
	int ey = uy.i>>23 & 0xff;
	uint32_t sx = ux.i & 0x80000000;
	uint32_t i;
	uint32_t uxi = ux.i;

	if (uy.i<<1 == 0 || isnan(y) || ex == 0xff)
		return (x*y)/(x*y);
	if (uxi<<1 <= uy.i<<1) {
		if (uxi<<1 == uy.i<<1)
			return 0*x;
		return x;
	}

	/* normalize x and y */
	if (!ex) {
		for (i = uxi<<9; i>>31 == 0; ex--, i <<= 1);
		uxi <<= -ex + 1;
	} else {
		uxi &= -1U >> 9;
		uxi |= 1U << 23;
	}
	if (!ey) {
		for (i = uy.i<<9; i>>31 == 0; ey--, i <<= 1);
		uy.i <<= -ey + 1;
	} else {
		uy.i &= -1U >> 9;
		uy.i |= 1U << 23;
	}

	/* x mod y */
	for (; ex > ey; ex--) {
		i = uxi - uy.i;
		if (i >> 31 == 0) {
			if (i == 0)
				return 0*x;
			uxi = i;
		}
		uxi <<= 1;
	}
	i = uxi - uy.i;
	if (i >> 31 == 0) {
		if (i == 0)
			return 0*x;
		uxi = i;
	}
	for (; uxi>>23 == 0; uxi <<= 1, ex--);

	/* scale result up */
	if (ex > 0) {
		uxi -= 1U << 23;
		uxi |= (uint32_t)ex << 23;
	} else {
		uxi >>= -ex + 1;
	}
	uxi |= sx;
	ux.i = uxi;
	return ux.f;
}
