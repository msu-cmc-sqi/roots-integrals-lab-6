#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include "numerical.h"

#ifndef EPS
#define EPS 1e-5f
#endif

#ifdef USE_NEWTON
#define ROOT_METHOD "Newton"
#else
#define ROOT_METHOD "Bisection"
#endif

static float get_root(func_t f, func_t g, func_t df, func_t dg,
                      float a, float b, float eps, int *iters)
{
#ifdef USE_NEWTON
    return root_newton(f, g, df, dg, a, b, eps, iters);
#else
    (void)df;
    (void)dg;
    return root_bisection(f, g, a, b, eps, iters);
#endif
}

static void print_help(const char *name)
{
    printf("Usage: %s [options]\n", name);
    printf("Options:\n");
    printf("  --help           show help\n");
    printf("  --roots          print intersection points\n");
    printf("  --iterations     print iteration counts\n");
    printf("  --test-root      run root tests\n");
    printf("  --test-integral  run integral tests\n");
    printf("  no options       print area\n");
    printf("  --test-root N A B EPS      поиск точки пересечения функций на [A, B]\n");
    printf("  --test-integral N A B EPS  вычисление интеграла функции N на [A, B]\n");
}

static void compute(float eps, int show_roots, int show_iterations)
{
    int it12 = 0, it13 = 0, it23 = 0;
    int ii1 = 0, ii2 = 0, ii3 = 0, ii4 = 0;

   /* 9 вариант:
   f1 = 3 / ((x - 1)^2 + 1), f2 = sqrt(x + 0.5), f3 = exp(-x).
   Точки пересечения:
   f1=f3 на [-0.5, 0.0], f2=f3 на [0.0, 0.5], f1=f2 на [1.5, 2.5]. */
   
    float x13 = get_root(f1, f3, df1, df3, -0.5f, 0.0f, eps, &it13);
    float x23 = get_root(f2, f3, df2, df3,  0.0f, 0.5f, eps, &it23);
    float x12 = get_root(f1, f2, df1, df2,  1.5f, 2.5f, eps, &it12);

    float y13 = f1(x13);
    float y23 = f2(x23);
    float y12 = f1(x12);

    float i1a = integral_rect(f1, x13, x23, eps, &ii1);
    float i3  = integral_rect(f3, x13, x23, eps, &ii2);
    float i1b = integral_rect(f1, x23, x12, eps, &ii3);
    float i2  = integral_rect(f2, x23, x12, eps, &ii4);
    float area = (i1a - i3) + (i1b - i2);

    printf("Method: %s, eps = %.0e\n", ROOT_METHOD, (double)eps);

    if (show_roots) {
        printf("Roots:\n");
        printf("  f1=f3: x = %.7f, y = %.7f\n", x13, y13);
        printf("  f2=f3: x = %.7f, y = %.7f\n", x23, y23);
        printf("  f1=f2: x = %.7f, y = %.7f\n", x12, y12);
    }

    if (show_iterations) {
        printf("Iterations:\n");
        printf("  root f1=f3: %d\n", it13);
        printf("  root f2=f3: %d\n", it23);
        printf("  root f1=f2: %d\n", it12);
        printf("  integral f1[x13,x23]: %d\n", ii1);
        printf("  integral f3[x13,x23]: %d\n", ii2);
        printf("  integral f1[x23,x12]: %d\n", ii3);
        printf("  integral f2[x23,x12]: %d\n", ii4);
    }

    printf("Area = %.7f\n", area);
}

static void test_root(void)
{
    int it = 0;
    float r;

    r = root_bisection(f1, f3, -0.5f, 0.0f, 1e-5f, &it);
    printf("bisection f1=f3: x=%.7f, expected about -0.2033349, iterations=%d\n", r, it);

    r = root_newton(f1, f3, df1, df3, -0.5f, 0.0f, 1e-5f, &it);
    printf("newton f1=f3:    x=%.7f, expected about -0.2033349, iterations=%d\n", r, it);

    r = root_bisection(f2, f3, 0.0f, 0.5f, 1e-5f, &it);
    printf("bisection f2=f3: x=%.7f, expected about 0.1874113, iterations=%d\n", r, it);

    r = root_newton(f2, f3, df2, df3, 0.0f, 0.5f, 1e-5f, &it);
    printf("newton f2=f3:    x=%.7f, expected about 0.1874113, iterations=%d\n", r, it);

    r = root_bisection(f1, f2, 1.5f, 2.5f, 1e-5f, &it);
    printf("bisection f1=f2: x=%.7f, expected about 1.9561525, iterations=%d\n", r, it);
}

static void test_integral(void)
{
    int it = 0;
    float res;

    res = integral_rect(f1, 0.0f, 1.0f, 1e-5f, &it);
    printf("integral f1[0,1] = %.7f, expected about 2.3561945, iterations=%d\n", res, it);

    res = integral_rect(f2, 0.0f, 2.0f, 1e-5f, &it);
    printf("integral f2[0,2] = %.7f, expected about 2.3995291, iterations=%d\n", res, it);

    res = integral_rect(f3, 0.0f, 1.0f, 1e-5f, &it);
    printf("integral f3[0,1] = %.7f, expected about 0.6321206, iterations=%d\n", res, it);
}

int main(int argc, char **argv)
{
    int show_roots = 0;
    int show_iterations = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--roots") == 0) {
            show_roots = 1;
        } else if (strcmp(argv[i], "--iterations") == 0) {
            show_iterations = 1;
        } else if (strcmp(argv[i], "--test-root") == 0) {
            test_root();
            return 0;
        } else if (strcmp(argv[i], "--test-integral") == 0) {
            test_integral();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    compute(EPS, show_roots, show_iterations);
    return 0;
}
