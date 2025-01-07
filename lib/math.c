#include <math.h>

float sqrtf(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    //x = x * (1.5f - xhalf * x * x);
    return 1.0f / x;
}


// Compute natural log using Taylor series
float logf(float x) {
    if (x <= 0) return 0.0f; // Handle invalid input

    // Use log(1+y) series where y = (x-1)/(x+1)
    float y = (x - 1.0f) / (x + 1.0f);
    float y2 = y * y;
    float sum = y;
    float term = y;

    // Unroll loop for better performance
    term = term * y2;
    sum += term / 3.0f;
    term = term * y2;
    sum += term / 5.0f;
    term = term * y2;
    sum += term / 7.0f;
    term = term * y2;
    sum += term / 9.0f;
    term = term * y2;
    sum += term / 11.0f;
    term = term * y2;
    sum += term / 13.0f;
    term = term * y2;
    sum += term / 15.0f;


    return 2.0f * sum;
}

// Compute e^x using Taylor series
float expf(float x) {
    float sum = 1.0f; // First term
    float term = 1.0f;
    float n = 1.0f;

    // Unroll loop for better performance
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;
    term = term * x / n; sum += term; n += 1.0f;


    return sum;
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
        float lnx = logf(x);
        float ylnx = y * lnx;
        if (ylnx > 88.0f) return 1e38f;  // Avoid overflow
        if (ylnx < -88.0f) return 1e-38f; // Avoid underflow
    }

    return expf(y * logf(x));
}
