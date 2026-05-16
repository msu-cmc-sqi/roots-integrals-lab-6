#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "root.h"
#include "integral.h"

extern long double f1(long double x);
extern long double f2(long double x);
extern long double f3(long double x);
extern long double df1(long double x);
extern long double df2(long double x);
extern long double df3(long double x);

/* Подынтегральные функции для двух частей площади */
static long double f1_minus_f3(long double x) { return f1(x) - f3(x); }
static long double f1_minus_f2(long double x) { return f1(x) - f2(x); }

/* Функции для тестов */
static long double tst_sq(long double x)   { return x * x; }
static long double tst_c2(long double x)   { (void)x; return 2.0L; }
static long double tst_dsq(long double x)  { return 2.0L * x; }
static long double tst_zero(long double x) { (void)x; return 0.0L; }
static long double tst_sin(long double x)  { return sinl(x); }
static long double tst_cos(long double x)  { return cosl(x); }
static long double tst_exp(long double x)  { return expl(x); }
static long double tst_inv(long double x)  { return 1.0L / x; }

/* Отрезки подобраны вручную так, чтобы F меняла знак на концах (подробнее - analys.md).
    f1-f3: F(0.20)<0, F(0.35)>0
    f2-f3: F(1.00)<0, F(1.80)>0
    f1-f2: F(2.40)>0, F(2.70)<0 */
#define A_LO  0.20L
#define A_HI  0.35L
#define B_LO  1.00L
#define B_HI  1.80L
#define C_LO  2.40L
#define C_HI  2.70L

static void print_help(const char *prog)
{
    printf("Использование: %s [опции]\n\n", prog);
    printf("  (без аргументов)    вычислить площадь фигуры (eps=1e-6)\n");
    printf("  --help              справка\n");
    printf("  --roots             точки пересечения кривых\n");
    printf("  --iterations        число итераций при поиске корней\n");
    printf("  --eps <значение>    точность вычислений (по умолчанию 1e-6)\n");
    printf("  --test-root         тест функции root()\n");
    printf("  --test-integral     тест функции integral()\n\n");
    printf("Вариант: f1=e^(-x)+3, f2=2x-2, f3=1/x\n");
    printf("Метод корней: ");
#ifdef METHOD_NEWTON
    printf("Ньютона\n");
#else
    printf("хорд (регула фальси)\n");
#endif
    printf("Метод интегрирования: Симпсон\n");
}

static int run_test_root(void)
{
    int ok = 1;
    printf("=== Тест root() ===\n");

    /* Тест 1: x^2 = 2 на [1, 2], корень = sqrt(2) ~ 1.41421356 */
    {
        int iters = 0;
        long double r = root(tst_sq, tst_c2, tst_dsq, tst_zero,
                             1.0L, 2.0L, 1e-10L, &iters);
        long double expected = sqrtl(2.0L);
        long double err = fabsl(r - expected);
        int pass = err < 1e-9L;
        printf("  x^2=2 на [1,2]:       корень=%.15Lf  ожид=%.15Lf  err=%.2Le  iter=%d  [%s]\n",
               r, expected, err, iters, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }
    /* Тест 2: sin(x) = 0 на [2, 4], корень = pi ~ 3.14159265 */
    {
        int iters = 0;
        long double r = root(tst_sin, tst_zero, tst_cos, tst_zero,
                             2.0L, 4.0L, 1e-10L, &iters);
        long double expected = acosl(-1.0L);
        long double err = fabsl(r - expected);
        int pass = err < 1e-9L;
        printf("  sin(x)=0 на [2,4]:    корень=%.15Lf  ожид=%.15Lf  err=%.2Le  iter=%d  [%s]\n",
               r, expected, err, iters, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }
    /* Тест 3: e^x = 2 на [0.5, 1.0], корень = ln(2) ~ 0.69314718 */
    {
        int iters = 0;
        long double r = root(tst_exp, tst_c2, tst_exp, tst_zero,
                             0.5L, 1.0L, 1e-10L, &iters);
        long double expected = logl(2.0L);
        long double err = fabsl(r - expected);
        int pass = err < 1e-9L;
        printf("  e^x=2 на [0.5,1]:     корень=%.15Lf  ожид=%.15Lf  err=%.2Le  iter=%d  [%s]\n",
               r, expected, err, iters, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }

    printf("  Итог: %s\n\n", ok ? "ВСЕ ТЕСТЫ ПРОЙДЕНЫ" : "ЕСТЬ ОШИБКИ");
    return ok;
}

static int run_test_integral(void)
{
    int ok = 1;
    printf("=== Тест integral() ===\n");

    /* Тест 1 (тривиальный): int[0,1] x^2 dx = 1/3
     * Симпсон точен для полиномов степени <= 3, поэтому ошибка машинная */
    {
        long double r = integral(tst_sq, 0.0L, 1.0L, 1e-12L);
        long double expected = 1.0L / 3.0L;
        long double err = fabsl(r - expected);
        int pass = err < 1e-10L;
        printf("  int[0,1] x^2 dx =      %.15Lf  ожид=%.15Lf  err=%.2Le  [%s]\n",
               r, expected, err, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }
    /* Тест 2: int[0,pi] sin(x) dx = 2 */
    {
        long double pi = acosl(-1.0L);
        long double r = integral(tst_sin, 0.0L, pi, 1e-12L);
        long double expected = 2.0L;
        long double err = fabsl(r - expected);
        int pass = err < 1e-10L;
        printf("  int[0,pi] sin(x) dx =  %.15Lf  ожид=%.15Lf  err=%.2Le  [%s]\n",
               r, expected, err, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }
    /* Тест 3: int[1,e] (1/x) dx = ln(e) - ln(1) = 1 */
    {
        long double e = expl(1.0L);
        long double r = integral(tst_inv, 1.0L, e, 1e-12L);
        long double expected = 1.0L;
        long double err = fabsl(r - expected);
        int pass = err < 1e-10L;
        printf("  int[1,e] (1/x) dx =    %.15Lf  ожид=%.15Lf  err=%.2Le  [%s]\n",
               r, expected, err, pass ? "OK" : "FAIL");
        if (!pass) ok = 0;
    }

    printf("  Итог: %s\n\n", ok ? "ВСЕ ТЕСТЫ ПРОЙДЕНЫ" : "ЕСТЬ ОШИБКИ");
    return ok;
}

int main(int argc, char *argv[])
{
    int show_roots = 0, show_iters = 0;
    int do_test_root = 0, do_test_integral = 0;
    long double eps = 1e-6L;

    for (int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "--help")          == 0) { print_help(argv[0]); return 0; }
        else if (strcmp(argv[i], "--roots")         == 0) show_roots       = 1;
        else if (strcmp(argv[i], "--iterations")    == 0) show_iters       = 1;
        else if (strcmp(argv[i], "--test-root")     == 0) do_test_root     = 1;
        else if (strcmp(argv[i], "--test-integral") == 0) do_test_integral = 1;
        else if (strcmp(argv[i], "--eps") == 0) {
            if (++i >= argc) { fprintf(stderr, "Нет значения для --eps\n"); return 1; }
            eps = strtold(argv[i], NULL);
        }
        else {
            fprintf(stderr, "Неизвестный ключ: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }

    if (do_test_root || do_test_integral) {
        int all_ok = 1;
        if (do_test_root)     all_ok &= run_test_root();
        if (do_test_integral) all_ok &= run_test_integral();
        return all_ok ? 0 : 1;
    }

    int iA = 0, iB = 0, iC = 0;
    long double xA = root(f1, f3, df1, df3, A_LO, A_HI, eps, &iA);  /* f1 ∩ f3 */
    long double xB = root(f2, f3, df2, df3, B_LO, B_HI, eps, &iB);  /* f2 ∩ f3 */
    long double xC = root(f1, f2, df1, df2, C_LO, C_HI, eps, &iC);  /* f1 ∩ f2 */

    if (show_roots) {
        printf("Точки пересечения (eps = %.0e):\n", (double)eps);
        printf("  f1 ∩ f3 (xA): x = %+.10Lf\n", xA);
        printf("  f2 ∩ f3 (xB): x = %+.10Lf\n", xB);
        printf("  f1 ∩ f2 (xC): x = %+.10Lf\n", xC);
    }
    if (show_iters) {
        printf("Итерации:\n");
        printf("  f1 ∩ f3: %d\n", iA);
        printf("  f2 ∩ f3: %d\n", iB);
        printf("  f1 ∩ f2: %d\n", iC);
    }

    /* S = int[xA,xB](f1-f3) + int[xB,xC](f1-f2) */
    long double area = integral(f1_minus_f3, xA, xB, eps)
                     + integral(f1_minus_f2, xB, xC, eps);

    printf("Площадь = %.10Lf\n", area);
    return 0;
}
