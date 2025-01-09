#include <math.h>

#define INFINITY 0x7f800000

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
