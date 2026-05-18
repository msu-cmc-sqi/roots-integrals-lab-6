#include <math.h>
#include "funcs.h"

int integral_iterations = 0;

double integral(
    double (*f)(double),
    double a,
    double b,
    double eps
)
{
    int n;
    int i;
    double h;
    double s1;
    double s2;

    n = 1;
    integral_iterations = 0;

    h = b - a;
    s1 = (f(a) + f(b)) * h / 2.0;

    do
    {
        s2 = s1;
        n *= 2;
        h = (b - a) / n;

        s1 = (f(a) + f(b)) / 2.0;

        for (i = 1; i < n; i++)
            s1 += f(a + i * h);

        s1 *= h;
        integral_iterations++;

    } while (fabs(s1 - s2) > eps);

    return s1;
}
