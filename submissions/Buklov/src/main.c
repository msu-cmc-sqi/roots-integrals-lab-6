/*
 * Лаб 6, вариант 9. Буклов М.А.
 *   f1 = 3 / ((x-1)^2 + 1)
 *   f2 = sqrt(x + 0.5)
 *   f3 = exp(-x)
 *
 * Фигура -- криволинейный треугольник на пересечениях f1, f2, f3.
 * Корень: деление пополам. Интеграл: средние прямоугольники + Рунге.
 */

#include <stdio.h>
#include <string.h>
#include "../include/funcs.h"

extern int g_iterations;

static const double EPS = 1e-5;

static void usage(void) {
    puts("usage: ./prog [--help|--roots|--iterations|--test-root|--test-integral]");
    puts("  без флагов -- посчитать площадь фигуры");
}

/* служебная f(x) = x, для теста интеграла */
static double identity(double x) { return x; }

/* нулевая функция -- чтобы тестировать root() как поиск нуля одной функции */
static double zero(double x) { (void)x; return 0; }

static void test_root(void) {
    /* x^2 - 2 = 0 на [1, 2] -- должен дать sqrt(2) ≈ 1.41421356 */
    double mul(double x) { return x * x - 2; }
    double r = root(mul, zero, 1.0, 2.0, 1e-8);
    printf("test root: sqrt(2) ≈ %.10f, итераций %d\n", r, g_iterations);
}

static void test_integral(void) {
    /* ∫₀¹ x dx = 0.5 */
    double r = integral(identity, 0, 1, 1e-8);
    printf("test integral: ∫₀¹ x dx ≈ %.10f (ожидаем 0.5)\n", r);
}

/* находит три вершины треугольника и при необходимости печатает их */
static void find_roots(double *x_ab, double *x_bc, double *x_ca, int verbose) {
    *x_ab = root(f1, f3, -0.5, 0.0, EPS);   /* f1 = f3 */
    *x_bc = root(f2, f3,  0.0, 1.0, EPS);   /* f2 = f3 */
    *x_ca = root(f1, f2,  1.0, 3.0, EPS);   /* f1 = f2 */

    if (verbose) {
        printf("f1 = f3 в x = %.8f\n", *x_ab);
        printf("f2 = f3 в x = %.8f\n", *x_bc);
        printf("f1 = f2 в x = %.8f\n", *x_ca);
    }
}

static void show_roots(void) {
    double a, b, c;
    find_roots(&a, &b, &c, 1);
}

static void show_iterations(void) {
    double a, b, c;
    root(f1, f3, -0.5, 0.0, EPS);  printf("f1 = f3: %d итераций\n", g_iterations);
    root(f2, f3,  0.0, 1.0, EPS);  printf("f2 = f3: %d итераций\n", g_iterations);
    root(f1, f2,  1.0, 3.0, EPS);  printf("f1 = f2: %d итераций\n", g_iterations);
    (void)a; (void)b; (void)c;
}

/* разности подынтегральных функций */
static double f1_minus_f3(double x) { return f1(x) - f3(x); }
static double f1_minus_f2(double x) { return f1(x) - f2(x); }

static void area(void) {
    double a, b, c;
    find_roots(&a, &b, &c, 0);

    /* слева сверху f1, снизу f3; справа сверху f1, снизу f2 */
    double s1 = integral(f1_minus_f3, a, b, EPS);
    double s2 = integral(f1_minus_f2, b, c, EPS);

    printf("вершины: A=%.6f  B=%.6f  C=%.6f\n", a, b, c);
    printf("S_AB (f1 - f3) = %.8f\n", s1);
    printf("S_BC (f1 - f2) = %.8f\n", s2);
    printf("S = %.8f\n", s1 + s2);
}

int main(int argc, char *argv[]) {
    if (argc == 1)                                  { area();           return 0; }
    if (strcmp(argv[1], "--help") == 0)             { usage();          return 0; }
    if (strcmp(argv[1], "--roots") == 0)            { show_roots();     return 0; }
    if (strcmp(argv[1], "--iterations") == 0)       { show_iterations();return 0; }
    if (strcmp(argv[1], "--test-root") == 0)        { test_root();      return 0; }
    if (strcmp(argv[1], "--test-integral") == 0)    { test_integral();  return 0; }

    printf("неизвестный ключ: %s\n", argv[1]);
    usage();
    return 1;
}
