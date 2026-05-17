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
#define MAX_ITERS 100000

static long root_iters = 0;
static long integral_iters = 0;

static real value(func_t f, func_t g, real x)
{
    return f(x) - g(x);
}

#ifdef USE_CHORD
static const char *method_name(void)
{
    return "chord";
}

real root(func_t f, func_t g, func_t df, func_t dg,
          real a, real b, real eps, real *x)
{
    real fa = value(f, g, a);
    real fb = value(f, g, b);

    (void)df;
    (void)dg;
    root_iters = 0;

    /* Chord method for equation f(x) - g(x) = 0. */
    while (fabsl(b - a) > eps && root_iters < MAX_ITERS) {
        *x = (a * fb - b * fa) / (fb - fa);

        if (value(f, g, a) * value(f, g, *x) <= 0.0L) {
            b = *x;
            fb = value(f, g, b);
        } else {
            a = *x;
            fa = value(f, g, a);
        }

        root_iters++;
    }

    return *x;
}
#else
static const char *method_name(void)
{
    return "newton";
}

real root(func_t f, func_t g, func_t df, func_t dg,
          real a, real b, real eps, real *x)
{
    real old_x;
    real derivative;

    *x = (a + b) / 2.0L;
    root_iters = 0;

    /* Newton method for equation f(x) - g(x) = 0. */
    do {
        old_x = *x;
        derivative = df(*x) - dg(*x);

        if (fabsl(derivative) > 1e-20L) {
            *x = *x - value(f, g, *x) / derivative;
        }

        /* If Newton jumps out of the segment, return to the middle. */
        if (*x <= a || *x >= b) {
            *x = (a + b) / 2.0L;
        }

        if (value(f, g, a) * value(f, g, *x) <= 0.0L) {
            b = *x;
        } else {
            a = *x;
        }

        root_iters++;
    } while (fabsl(*x - old_x) > eps &&
             fabsl(value(f, g, *x)) > eps &&
             root_iters < MAX_ITERS);

    return *x;
}
#endif

real integral(func_t f, real a, real b, real eps)
{
    int n = 2;
    real old_sum = 0.0L;
    real new_sum = 0.0L;

    integral_iters = 0;

    while (1) {
        real h = (b - a) / n;
        real sum = f(a) + f(b);

        for (int i = 1; i < n; i++) {
            real x = a + h * i;

            if (i % 2 == 0) {
                sum += 2.0L * f(x);
            } else {
                sum += 4.0L * f(x);
            }
        }

        new_sum = sum * h / 3.0L;
        integral_iters++;

        /* Runge rule for Simpson formula. */
        if (integral_iters > 1 && fabsl(new_sum - old_sum) / 15.0L < eps) {
            break;
        }

        old_sum = new_sum;
        n *= 2;

        if (n > (1 << 26)) {
            break;
        }
    }

    return new_sum;
}

static real f3_minus_f1(real x)
{
    return f3(x) - f1(x);
}

static real f2_minus_f1(real x)
{
    return f2(x) - f1(x);
}

static void print_help(const char *program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  --help           print this help\n");
    printf("  --roots          print intersection points\n");
    printf("  --iterations     print iteration counts\n");
    printf("  --test-root      test root function\n");
    printf("  --test-integral  test integral function\n");
    printf("  --test           run all tests\n");
}

static int test_root(void)
{
    real x = 0.0L;
    real answer = (4.0L - sqrtl(26.0L)) / 2.0L;

    root(f2, f3, df2, df3, -1.0L, -0.1L, EPS1, &x);
    printf("root test: got %.12Lf, expected %.12Lf\n", x, answer);

    return fabsl(x - answer) < 1e-7L;
}

static int test_integral(void)
{
    real answer = integral(f2, 0.0L, 1.0L, EPS2);

    printf("integral test: got %.12Lf, expected 7.000000000000\n", answer);

    return fabsl(answer - 7.0L) < 1e-7L;
}

int main(int argc, char **argv)
{
    real x13 = 0.0L;
    real x23 = 0.0L;
    real x12 = 0.0L;
    real left_area;
    real right_area;
    real area;
    long it13;
    long it23;
    long it12;
    long left_it;
    long right_it;

    /* Intersections are searched on fixed segments for variant 8. */
    root(f1, f3, df1, df3, -3.0L, -2.0L, EPS1, &x13);
    it13 = root_iters;

    root(f2, f3, df2, df3, -1.0L, -0.1L, EPS1, &x23);
    it23 = root_iters;

    root(f1, f2, df1, df2, 1.0L, 2.0L, EPS1, &x12);
    it12 = root_iters;

    left_area = integral(f3_minus_f1, x13, x23, EPS2);
    left_it = integral_iters;

    right_area = integral(f2_minus_f1, x23, x12, EPS2);
    right_it = integral_iters;

    area = left_area + right_area;

    if (argc == 1) {
        printf("Variant 8, method: %s\n", method_name());
        printf("x13 = %.12Lf (f1 = f3)\n", x13);
        printf("x23 = %.12Lf (f2 = f3)\n", x23);
        printf("x12 = %.12Lf (f1 = f2)\n", x12);
        printf("area = %.12Lf\n", area);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
        } else if (strcmp(argv[i], "--roots") == 0) {
            printf("x13 = %.12Lf (f1 = f3)\n", x13);
            printf("x23 = %.12Lf (f2 = f3)\n", x23);
            printf("x12 = %.12Lf (f1 = f2)\n", x12);
        } else if (strcmp(argv[i], "--iterations") == 0) {
            printf("f1=f3 root iterations: %ld\n", it13);
            printf("f2=f3 root iterations: %ld\n", it23);
            printf("f1=f2 root iterations: %ld\n", it12);
            printf("left integral iterations: %ld\n", left_it);
            printf("right integral iterations: %ld\n", right_it);
        } else if (strcmp(argv[i], "--test-root") == 0) {
            if (!test_root()) {
                return 1;
            }
        } else if (strcmp(argv[i], "--test-integral") == 0) {
            if (!test_integral()) {
                return 1;
            }
        } else if (strcmp(argv[i], "--test") == 0) {
            if (!test_root() || !test_integral()) {
                return 1;
            }
            printf("all tests passed\n");
        } else {
            printf("unknown option: %s\n", argv[i]);
            printf("try --help\n");
            return 1;
        }
    }

    return 0;
}
