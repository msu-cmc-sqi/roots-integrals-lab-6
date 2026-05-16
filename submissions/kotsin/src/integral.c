#include <math.h>
#include "integral.h"

/*  Метод Симпсона с последовательным удвоением числа панелей
    Стоп, когда оценка погрешности по Рунге: |S_{2n} - S_n| / 15 < eps */
long double integral(
    long double (*f)(long double),
    long double a, long double b,
    long double eps)
{
    int n = 2;
    long double h  = (b - a) / 2.0L;
    long double Sn = (f(a) + 4.0L * f(a + h) + f(b)) * h / 3.0L;

    for (;;) {
        n *= 2;
        h  = (b - a) / n;
        long double S2n = f(a) + f(b);
        for (int i = 1; i < n; i++)
            S2n += (i % 2 == 0 ? 2.0L : 4.0L) * f(a + i * h);
        S2n *= h / 3.0L;

        if (fabsl(S2n - Sn) / 15.0L < eps)
            return S2n;

        Sn = S2n;
        if (n >= (1 << 22))
            return S2n;
    }
}
