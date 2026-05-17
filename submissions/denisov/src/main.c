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

#define EPS_ROOT 1e-8L
#define EPS_INTEGRAL 1e-8L
#define MAX_ITERS 100000L

typedef struct {
    real x13;
    real x23;
    real x12;
    long it13;
    long it23;
    long it12;
} roots_t;

typedef struct {
    real left;
    real right;
    real total;
    long left_iters;
    long right_iters;
} area_t;

static long root_iterations;
static long integral_iterations;

static real diff(func_t f, func_t g, real x)
{
    return f(x) - g(x);
}

static real ddiff(func_t df, func_t dg, real x)
{
    return df(x) - dg(x);
}

static real root_chord(func_t f, func_t g, real a, real b, real eps)
{
    real fa = diff(f, g, a);
    real fb = diff(f, g, b);
    real x = a;

    root_iterations = 0;
    while (fabsl(b - a) > eps && root_iterations < MAX_ITERS) {
        x = (a * fb - b * fa) / (fb - fa);
        real fx = diff(f, g, x);
        ++root_iterations;

        if (fabsl(fx) < eps) {
            break;
        }

        if (fa * fx <= 0.0L) {
            b = x;
            fb = fx;
        } else {
            a = x;
            fa = fx;
        }
    }

    return x;
}

static real root_newton(func_t f, func_t g, func_t df, func_t dg,
                        real a, real b, real eps)
{
    real x = (a + b) / 2.0L;

    root_iterations = 0;
    while (root_iterations < MAX_ITERS) {
        real prev = x;
        real derivative = ddiff(df, dg, x);

        if (fabsl(derivative) < 1e-20L) {
            x = (a + b) / 2.0L;
        } else {
            x = x - diff(f, g, x) / derivative;
        }

        if (x <= a || x >= b) {
            x = (a + b) / 2.0L;
        }

        if (diff(f, g, a) * diff(f, g, x) <= 0.0L) {
            b = x;
        } else {
            a = x;
        }

        ++root_iterations;
        if (fabsl(x - prev) <= eps || fabsl(diff(f, g, x)) <= eps) {
            break;
        }
    }

    return x;
}

real root(func_t f, func_t g, func_t df, func_t dg,
          real a, real b, real eps, real *x)
{
#ifdef USE_CHORD
    (void)df;
    (void)dg;
    *x = root_chord(f, g, a, b, eps);
#else
    *x = root_newton(f, g, df, dg, a, b, eps);
#endif
    return *x;
}

real integral(func_t f, real a, real b, real eps)
{
    int n = 2;
    real prev = 0.0L;
    real cur = 0.0L;

    integral_iterations = 0;
    do {
        prev = cur;
        cur = f(a) + f(b);
        real h = (b - a) / n;

        for (int i = 1; i < n; ++i) {
            real x = a + h * i;
            cur += (i % 2 ? 4.0L : 2.0L) * f(x);
        }

        cur *= h / 3.0L;
        n *= 2;
        ++integral_iterations;
    } while ((integral_iterations == 1 ||
              fabsl(cur - prev) / 15.0L > eps) &&
             n < (1 << 26));

    return cur;
}

static real f3_minus_f1(real x)
{
    return f3(x) - f1(x);
}

static real f2_minus_f1(real x)
{
    return f2(x) - f1(x);
}

static roots_t find_roots(void)
{
    roots_t roots;

    root(f1, f3, df1, df3, -3.0L, -2.0L, EPS_ROOT, &roots.x13);
    roots.it13 = root_iterations;

    root(f2, f3, df2, df3, -1.0L, -0.1L, EPS_ROOT, &roots.x23);
    roots.it23 = root_iterations;

    root(f1, f2, df1, df2, 1.0L, 2.0L, EPS_ROOT, &roots.x12);
    roots.it12 = root_iterations;

    return roots;
}

static area_t find_area(const roots_t *roots)
{
    area_t area;

    area.left = integral(f3_minus_f1, roots->x13, roots->x23, EPS_INTEGRAL);
    area.left_iters = integral_iterations;

    area.right = integral(f2_minus_f1, roots->x23, roots->x12, EPS_INTEGRAL);
    area.right_iters = integral_iterations;

    area.total = area.left + area.right;
    return area;
}

static const char *method_name(void)
{
#ifdef USE_CHORD
    return "chord";
#else
    return "newton";
#endif
}

static void print_help(const char *prog)
{
    printf("Usage: %s [options]\n", prog);
    printf("Options:\n");
    printf("  --help           print this help\n");
    printf("  --roots          print intersection points\n");
    printf("  --iterations     print iteration counts\n");
    printf("  --test-root      run root() test\n");
    printf("  --test-integral  run integral() test\n");
    printf("  --test           run all tests\n");
    printf("\nVariant 8:\n");
    printf("  f1(x) = exp(x) + 2\n");
    printf("  f2(x) = -2x + 8\n");
    printf("  f3(x) = -5 / x\n");
    printf("  root method: %s\n", method_name());
    printf("  integral method: Simpson formula\n");
}

static void print_roots(const roots_t *roots)
{
    printf("x13 = %.12Lf (f1 = f3)\n", roots->x13);
    printf("x23 = %.12Lf (f2 = f3)\n", roots->x23);
    printf("x12 = %.12Lf (f1 = f2)\n", roots->x12);
}

static void print_iterations(const roots_t *roots, const area_t *area)
{
    printf("f1=f3 root iterations: %ld\n", roots->it13);
    printf("f2=f3 root iterations: %ld\n", roots->it23);
    printf("f1=f2 root iterations: %ld\n", roots->it12);
    printf("left integral iterations: %ld\n", area->left_iters);
    printf("right integral iterations: %ld\n", area->right_iters);
}

static int test_root(void)
{
    real x;
    real exact = (4.0L - sqrtl(26.0L)) / 2.0L;

    root(f2, f3, df2, df3, -1.0L, -0.1L, EPS_ROOT, &x);
    printf("root test: got %.12Lf, expected %.12Lf\n", x, exact);
    return fabsl(x - exact) < 1e-7L ? 0 : 1;
}

static int test_integral(void)
{
    real res = integral(f2, 0.0L, 1.0L, EPS_INTEGRAL);

    printf("integral test: got %.12Lf, expected 7.000000000000\n", res);
    return fabsl(res - 7.0L) < 1e-7L ? 0 : 1;
}

static int run_tests(void)
{
    int failed = 0;

    failed += test_root();
    failed += test_integral();
    if (failed == 0) {
        printf("all tests passed\n");
    }
    return failed ? 1 : 0;
}

int main(int argc, char **argv)
{
    roots_t roots = find_roots();
    area_t area = find_area(&roots);

    if (argc == 1) {
        printf("Variant 8, method: %s\n", method_name());
        print_roots(&roots);
        printf("area = %.12Lf\n", area.total);
        return 0;
    }

    int exit_code = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
        } else if (strcmp(argv[i], "--roots") == 0) {
            print_roots(&roots);
        } else if (strcmp(argv[i], "--iterations") == 0) {
            print_iterations(&roots, &area);
        } else if (strcmp(argv[i], "--test-root") == 0) {
            exit_code |= test_root();
        } else if (strcmp(argv[i], "--test-integral") == 0) {
            exit_code |= test_integral();
        } else if (strcmp(argv[i], "--test") == 0) {
            exit_code |= run_tests();
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[i]);
            fprintf(stderr, "try --help\n");
            return 1;
        }
    }

    return exit_code;
}
