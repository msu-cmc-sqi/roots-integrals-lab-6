#include <math.h>
#include <stdio.h>

typedef float (*func_t)(float);

static float h(func_t f, func_t g, float x) { return f(x) - g(x); }

int root_chord(func_t f, func_t g, float a, float b, float eps, int *iters, float *x) {
    float fa = h(f, g, a), fb = h(f, g, b);
    if (fa * fb > 0) return -1;
    int k = 0;

    float c_prev = a;
    while (1) {
        float c = b - (fb * (b - a)) / (fb - fa);
        float fc = h(f, g, c);
        
        if (fabsf(c - c_prev) < eps) {
            *x = c;
            break;
        }
        
        c_prev = c;
        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
        k++;
        if (k > 100000) { *x = c; break; }
    }

    if (iters) *iters = k;
    return 0;
}

int root_combined(func_t f, func_t g, float a, float b, float eps, int *iters, float *x) {
    float fa = h(f, g, a), fb = h(f, g, b);
    if (fa * fb > 0) return -1;
    int k = 0;

    while (fabsf(b - a) > eps && k < 100000) {
        // Шаг хорды с левого конца
        float c = a - (fa * (b - a)) / (fb - fa);
        
        // Шаг касательной (численное приближение производной)
        float dx = 1e-4f;
        float df = (h(f, g, b + dx) - h(f, g, b)) / dx;
        float m = b - h(f, g, b) / df;

        a = c; // Хорда двигает левую границу 'a'
        b = m; // Касательная двигает правую границу 'b'
        
        fa = h(f, g, a);
        fb = h(f, g, b);
        k++;
    }
    *x = 0.5f * (a + b);
    if (iters) *iters = k;
    return 0;
}

float integral_simpson(func_t f, float a, float b, float eps) {
    int n = 2;
    float prev = 0.0f;
    while (1) {
        float h = (b - a) / n;
        float s1 = 0.0f, s2 = 0.0f;

        for (int i = 1; i < n; i += 2) s1 += f(a + i * h);
        for (int i = 2; i < n; i += 2) s2 += f(a + i * h);

        float cur = (h / 3.0f) * (f(a) + f(b) + 4.0f * s1 + 2.0f * s2);
        
        if (n > 2 && fabsf(cur - prev) < eps) return cur;

        prev = cur;
        n *= 2;
        if (n > (1 << 22)) return cur;
    }
}
