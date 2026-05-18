#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "funcs.h"

#define EPS_DEFAULT 1e-5

double f3_minus_f1(double x)
{
    return f3(x) - f1(x);
}

double f2_minus_f1(double x)
{
    return f2(x) - f1(x);
}

double test_x(double x)
{
    return x;
}

double test_x2(double x)
{
    return x * x;
}

double test_const_4(double x)
{
    (void)x;
    return 4.0;
}

void print_help(void)
{
    printf("Usage:\n");
    printf("  ./main [eps]\n");
    printf("  ./main --help\n");
    printf("  ./main --roots [eps]\n");
    printf("  ./main --iterations [eps]\n");
    printf("  ./main --test-root\n");
    printf("  ./main --test-integral\n");
    printf("\n");
    printf("Variant 7:\n");
    printf("  f1(x) = ln(x)\n");
    printf("  f2(x) = -2x + 14\n");
    printf("  f3(x) = 1/(2-x) + 6\n");
}

void compute_points(double eps, double *x13, double *x23, double *x12,
                    int *it13, int *it23, int *it12)
{
    *x13 = root(f1, f3, 2.1, 2.3, eps);
    *it13 = root_iterations;

    *x23 = root(f2, f3, 4.0, 4.5, eps);
    *it23 = root_iterations;

    *x12 = root(f1, f2, 6.0, 6.2, eps);
    *it12 = root_iterations;
}

void print_points(double x13, double x23, double x12)
{
    printf("f1 = f3: x13 = %.10lf\n", x13);
    printf("f2 = f3: x23 = %.10lf\n", x23);
    printf("f1 = f2: x12 = %.10lf\n", x12);
}

void print_iterations(int it13, int it23, int it12, int it_int1, int it_int2)
{
    printf("Root iterations:\n");
    printf("  f1 = f3: %d\n", it13);
    printf("  f2 = f3: %d\n", it23);
    printf("  f1 = f2: %d\n", it12);
    printf("Integral iterations:\n");
    printf("  integral 1: %d\n", it_int1);
    printf("  integral 2: %d\n", it_int2);
}

int test_root(void)
{
    double x;

    x = root(test_x2, test_const_4, 1.0, 3.0, 1e-7);

    printf("Test root:\n");
    printf("  equation: x^2 = 4\n");
    printf("  result:   %.10lf\n", x);
    printf("  expected: %.10lf\n", 2.0);

    if (fabs(x - 2.0) < 1e-5)
    {
        printf("  status: OK\n");
        return 1;
    }

    printf("  status: FAILED\n");
    return 0;
}

int test_integral(void)
{
    double res;

    res = integral(f1, 1.0, 2.0, 1e-7);

    printf("Test integral:\n");
    printf("  integral ln(x) from 1 to 2\n");
    printf("  result:   %.10lf\n", res);
    printf("  expected: %.10lf\n", 2.0 * log(2.0) - 1.0);

    if (fabs(res - (2.0 * log(2.0) - 1.0)) < 1e-5)
    {
        printf("  status: OK\n");
        return 1;
    }

    printf("  status: FAILED\n");
    return 0;
}

int main(int argc, char **argv)
{
    double eps;
    double x13;
    double x23;
    double x12;
    double s1;
    double s2;
    double s;
    int it13;
    int it23;
    int it12;
    int it_int1;
    int it_int2;

    eps = EPS_DEFAULT;

    if (argc > 2)
        eps = atof(argv[2]);
    else if (argc == 2 && argv[1][0] != '-')
        eps = atof(argv[1]);

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        print_help();
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "--test-root") == 0)
    {
        if (test_root())
            return 0;
        return 1;
    }

    if (argc > 1 && strcmp(argv[1], "--test-integral") == 0)
    {
        if (test_integral())
            return 0;
        return 1;
    }

    if (argc > 1 && strcmp(argv[1], "--test") == 0)
    {
        if (test_root() && test_integral())
            return 0;
        return 1;
    }

    compute_points(eps, &x13, &x23, &x12, &it13, &it23, &it12);

    if (argc > 1 && (strcmp(argv[1], "--roots") == 0 || strcmp(argv[1], "--points") == 0))
    {
        print_points(x13, x23, x12);
        return 0;
    }

    s1 = integral(f3_minus_f1, x13, x23, eps);
    it_int1 = integral_iterations;

    s2 = integral(f2_minus_f1, x23, x12, eps);
    it_int2 = integral_iterations;

    s = s1 + s2;

    if (argc > 1 && strcmp(argv[1], "--iterations") == 0)
    {
        print_iterations(it13, it23, it12, it_int1, it_int2);
        return 0;
    }

    printf("eps = %.10g\n", eps);
    print_points(x13, x23, x12);
    printf("Area = %.10lf\n", s);

    return 0;
}
