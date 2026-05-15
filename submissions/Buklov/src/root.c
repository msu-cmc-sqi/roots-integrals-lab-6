#include <math.h>
#include "../include/funcs.h"

/* счётчик итераций последнего вызова root() -- читается из main для --iterations */
int g_iterations = 0;

/* выбор метода на этапе компиляции:
     -DMETHOD_CHORDS    -- метод хорд
     по умолчанию       -- метод деления отрезка пополам
   предполагается, что F(x) = f(x) - g(x) меняет знак на [a, b]. */

#ifdef METHOD_CHORDS

/* метод хорд для уравнения f(x) = g(x).
   на каждом шаге проводим хорду между концами и берём её пересечение с осью.
   тот конец, где знак F сохраняется, заменяем на найденную точку. */
double root(double (*f)(double), double (*g)(double),
            double a, double b, double eps) {
    g_iterations = 0;

    double Fa = f(a) - g(a);
    double Fb = f(b) - g(b);
    double x_prev = a;
    double x;

    while (1) {
        x = a - Fa * (b - a) / (Fb - Fa);
        double Fx = f(x) - g(x);

        g_iterations++;

        if (fabs(x - x_prev) < eps) return x;

        if (Fa * Fx < 0) {
            b  = x;
            Fb = Fx;
        } else {
            a  = x;
            Fa = Fx;
        }
        x_prev = x;
    }
}

#else

/* метод деления отрезка пополам для уравнения f(x) = g(x). */
double root(double (*f)(double), double (*g)(double),
            double a, double b, double eps) {
    g_iterations = 0;

    double Fa = f(a) - g(a);

    while (b - a > eps) {
        double mid  = (a + b) / 2;
        double Fmid = f(mid) - g(mid);

        if (Fa * Fmid < 0) {
            b = mid;
        } else {
            a  = mid;
            Fa = Fmid;
        }
        g_iterations++;
    }

    return (a + b) / 2;
}

#endif
