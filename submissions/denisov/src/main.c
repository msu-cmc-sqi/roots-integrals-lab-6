#include <math.h>
#include <stdio.h>
#include <string.h>

typedef long double real;
typedef real (*func_t)(real);

extern real f1(real x);
extern real f2(real x);
extern real f3(real x);
extern real df1(real x);
extern real df2(real x);
extern real df3(real x);

#define EPS1 1e-8L
#define EPS2 1e-8L

long root_iters;
long integral_iters;

real root(func_t f, func_t g, func_t df, func_t dg,
          real a, real b, real eps, real *x)
{
    real fa, fx;

    root_iters = 0;

#ifdef USE_CHORD
    real fb;

    (void)df;
    (void)dg;

    fa = f(a) - g(a);
    fb = f(b) - g(b);

    /* Method of chords. */
    while (fabsl(b - a) > eps && root_iters < 100000) {
        *x = (a * fb - b * fa) / (fb - fa);
        fx = f(*x) - g(*x);

        if (fa * fx <= 0.0L) {
            b = *x;
            fb = fx;
        } else {
            a = *x;
            fa = fx;
        }

        root_iters++;
    }
#else
    real dfx, old_x;

    /* Newton method. */
    *x = (a + b) / 2.0L;

    do {
        old_x = *x;
        fx = f(*x) - g(*x);
        dfx = df(*x) - dg(*x);

        if (fabsl(dfx) > 1e-20L) {
            *x = *x - fx / dfx;
        } else {
            *x = (a + b) / 2.0L;
        }

        if (*x < a || *x > b) {
            *x = (a + b) / 2.0L;
        }

        fa = f(a) - g(a);
        fx = f(*x) - g(*x);

        if (fa * fx <= 0.0L) {
            b = *x;
        } else {
            a = *x;
        }

        root_iters++;
    } while (fabsl(*x - old_x) > eps &&
             fabsl(f(*x) - g(*x)) > eps &&
             root_iters < 100000);
#endif

    return *x;
}

real integral(func_t f, real a, real b, real eps)
{
    int n = 2;
    int i;
    real h, x;
    real sum;
    real old_res = 0.0L;
    real res = 0.0L;

    integral_iters = 0;

    while (1) {
        h = (b - a) / n;
        sum = f(a) + f(b);

        /* Simpson formula: coefficients 4, 2, 4, 2, ... */
        for (i = 1; i < n; i++) {
            x = a + i * h;
            if (i % 2 == 0) {
                sum += 2.0L * f(x);
            } else {
                sum += 4.0L * f(x);
            }
        }

        res = sum * h / 3.0L;
        integral_iters++;

        if (integral_iters > 1 && fabsl(res - old_res) / 15.0L < eps) {
            break;
        }

        old_res = res;
        n = n * 2;

        if (n > 10000000) {
            break;
        }
    }

    return res;
}

real f3_f1(real x)
{
    return f3(x) - f1(x);
}

real f2_f1(real x)
{
    return f2(x) - f1(x);
}

void print_help(void)
{
    printf("Options:\n");
    printf("  --help\n");
    printf("  --roots\n");
    printf("  --iterations\n");
    printf("  --test-root\n");
    printf("  --test-integral\n");
    printf("  --test\n");
}

int test_root(void)
{
    real x;
    real ans;

    ans = (4.0L - sqrtl(26.0L)) / 2.0L;
    root(f2, f3, df2, df3, -1.0L, -0.1L, EPS1, &x);

    printf("root test: got %.12Lf, expected %.12Lf\n", x, ans);

    if (fabsl(x - ans) < 1e-7L) {
        return 1;
    }

    return 0;
}

int test_integral(void)
{
    real s;

    s = integral(f2, 0.0L, 1.0L, EPS2);
    printf("integral test: got %.12Lf, expected 7.000000000000\n", s);

    if (fabsl(s - 7.0L) < 1e-7L) {
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    real x13, x23, x12;
    real s1, s2, s;
    long it13, it23, it12;
    long int_it1, int_it2;
    int i;

    /* Roots of intersections for variant 8. */
    root(f1, f3, df1, df3, -3.0L, -2.0L, EPS1, &x13);
    it13 = root_iters;

    root(f2, f3, df2, df3, -1.0L, -0.1L, EPS1, &x23);
    it23 = root_iters;

    root(f1, f2, df1, df2, 1.0L, 2.0L, EPS1, &x12);
    it12 = root_iters;

    /* The whole area is the sum of two parts. */
    s1 = integral(f3_f1, x13, x23, EPS2);
    int_it1 = integral_iters;

    s2 = integral(f2_f1, x23, x12, EPS2);
    int_it2 = integral_iters;

    s = s1 + s2;

    if (argc == 1) {
#ifdef USE_CHORD
        printf("method: chord\n");
#else
        printf("method: newton\n");
#endif
        printf("x13 = %.12Lf\n", x13);
        printf("x23 = %.12Lf\n", x23);
        printf("x12 = %.12Lf\n", x12);
        printf("area = %.12Lf\n", s);
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
        } else if (strcmp(argv[i], "--roots") == 0) {
            printf("x13 = %.12Lf\n", x13);
            printf("x23 = %.12Lf\n", x23);
            printf("x12 = %.12Lf\n", x12);
        } else if (strcmp(argv[i], "--iterations") == 0) {
            printf("x13 iterations: %ld\n", it13);
            printf("x23 iterations: %ld\n", it23);
            printf("x12 iterations: %ld\n", it12);
            printf("first integral iterations: %ld\n", int_it1);
            printf("second integral iterations: %ld\n", int_it2);
        } else if (strcmp(argv[i], "--test-root") == 0) {
            if (!test_root()) {
                return 1;
            }
        } else if (strcmp(argv[i], "--test-integral") == 0) {
            if (!test_integral()) {
                return 1;
            }
        } else if (strcmp(argv[i], "--test") == 0) {
            if (!test_root()) {
                return 1;
            }
            if (!test_integral()) {
                return 1;
            }
            printf("all tests passed\n");
        }
    }

    return 0;
}
