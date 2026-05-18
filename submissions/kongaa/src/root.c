#include <math.h>
#include "funcs.h"

int root_iterations = 0;

static double diff(double (*f)(double), double (*g)(double), double x)
{
    return f(x) - g(x);
}

#ifdef COMBINED

double root(
    double (*f)(double),
    double (*g)(double),
    double a,
    double b,
    double eps
)
{
    double x0;
    double x1;
    double fa;
    double fb;
    double fx0;
    double fx1;

    root_iterations = 0;

    fa = diff(f, g, a);
    fb = diff(f, g, b);

    while (fabs(b - a) > eps)
    {
        x0 = (a + b) / 2.0;
        fx0 = diff(f, g, x0);

        x1 = (a * fb - b * fa) / (fb - fa);
        fx1 = diff(f, g, x1);

        if (fa * fx0 <= 0.0)
        {
            b = x0;
            fb = fx0;
        }
        else
        {
            a = x0;
            fa = fx0;
        }

        if (x1 > a && x1 < b)
        {
            fx1 = diff(f, g, x1);

            if (fa * fx1 <= 0.0)
            {
                b = x1;
                fb = fx1;
            }
            else
            {
                a = x1;
                fa = fx1;
            }
        }

        root_iterations++;
    }

    return (a + b) / 2.0;
}

#else

double root(
    double (*f)(double),
    double (*g)(double),
    double a,
    double b,
    double eps
)
{
    double c;
    double fa;
    double fc;

    root_iterations = 0;

    while (fabs(b - a) > eps)
    {
        c = (a + b) / 2.0;

        fa = diff(f, g, a);
        fc = diff(f, g, c);

        if (fa * fc <= 0.0)
            b = c;
        else
            a = c;

        root_iterations++;
    }

    return (a + b) / 2.0;
}

#endif
