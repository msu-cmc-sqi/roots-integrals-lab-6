#include <math.h>
#include "root.h"

#define MAX_ITER 100000

#ifdef METHOD_NEWTON
/* Метод Ньютона: x_{n+1} = x_n - F(x_n)/F'(x_n), F(x) = f(x) - g(x).
 * Стартуем с середины отрезка [a, b]. */
long double root(
    long double (*f)(long double),
    long double (*g)(long double),
    long double (*df)(long double),
    long double (*dg)(long double),
    long double a, long double b,
    long double eps, int *iters)
{
    *iters = 0;
    long double x = (a + b) / 2.0L;
    long double xprev;
    do {
        xprev = x;
        x -= (f(x) - g(x)) / (df(x) - dg(x));
        (*iters)++;
    } while (fabsl(x - xprev) >= eps && *iters < MAX_ITER);
    return x;
}

#else
/* Метод хорд: на каждом шаге двигаем тот конец отрезка,
   у которого знак F совпадает со знаком F в новой точке. */
long double root(
    long double (*f)(long double),
    long double (*g)(long double),
    long double (*df)(long double),
    long double (*dg)(long double),
    long double a, long double b,
    long double eps, int *iters)
{
    (void)df; (void)dg;
    *iters = 0;
    long double xa = a, xb = b;
    long double Fa = f(xa) - g(xa);
    long double Fb = f(xb) - g(xb);
    long double x = xa, Fx;
    do {
        x  = xa - Fa * (xb - xa) / (Fb - Fa);
        Fx = f(x) - g(x);
        if (Fa * Fx < 0.0L) { xb = x; Fb = Fx; }
        else                 { xa = x; Fa = Fx; }
        (*iters)++;
    } while (fabsl(Fx) >= eps && *iters < MAX_ITER);
    return x;
}
#endif
