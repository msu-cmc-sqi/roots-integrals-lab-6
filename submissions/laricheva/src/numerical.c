#include <math.h>
#include "numerical.h"

static float diff(func_t f, func_t g, float x)
{
    return f(x) - g(x);
}

float root_bisection(func_t f, func_t g, float a, float b, float eps, int *iters)
{
    int n = 0;
    float fa = diff(f, g, a);
    float fb = diff(f, g, b);

    if (fa == 0.0f) {
        if (iters) *iters = 0;
        return a;
    }
    if (fb == 0.0f) {
        if (iters) *iters = 0;
        return b;
    }

    while (fabsf(b - a) > 2.0f * eps && n < 100000) {
        float c = (a + b) / 2.0f;
        float fc = diff(f, g, c);

        if (fa * fc <= 0.0f) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
        n++;
    }

    if (iters) *iters = n;
    return (a + b) / 2.0f;
}

float root_newton(func_t f, func_t g, func_t df, func_t dg, float a, float b, float eps, int *iters)
{
    int n = 0;
    float x = (a + b) / 2.0f;

    while (n < 100000) {
        float fx = f(x) - g(x);
        float dfx = df(x) - dg(x);

        if (fabsf(dfx) < 1e-12f) {
            break;
        }

        float nx = x - fx / dfx;

        if (nx < a || nx > b || !isfinite(nx)) {
            nx = (a + b) / 2.0f;
        }

        if (diff(f, g, a) * diff(f, g, nx) <= 0.0f) {
            b = nx;
        } else {
            a = nx;
        }

        if (fabsf(nx - x) <= eps) {
            x = nx;
            n++;
            break;
        }

        x = nx;
        n++;
    }

    if (iters) *iters = n;
    return x;
}

static float rect_fixed(func_t f, float a, float b, int n)
{
    float h = (b - a) / (float)n;
    float sum = 0.0f;

    for (int i = 0; i < n; i++) {
        float x = a + ((float)i + 0.5f) * h;
        sum += f(x);
    }

    return sum * h;
}

float integral_rect(func_t f, float a, float b, float eps, int *iters)
{
    int n = 4;
    int k = 0;
    float prev = rect_fixed(f, a, b, n);

    while (k < 30) {
        n *= 2;
        float cur = rect_fixed(f, a, b, n);

        if (fabsf(cur - prev) / 3.0f <= eps) {
            if (iters) *iters = k + 1;
            return cur;
        }

        prev = cur;
        k++;
    }

    if (iters) *iters = k;
    return prev;
}
