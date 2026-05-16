#ifndef ROOT_H
#define ROOT_H

/* Ищет корень F(x) = f(x) - g(x) = 0 на [a, b] с точностью eps
   df, dg - производные f и g, нужны только для метода Ньютона
   *iters - счётчик итераций, обнуляется внутри функции */
long double root(
    long double (*f)(long double),
    long double (*g)(long double),
    long double (*df)(long double),
    long double (*dg)(long double),
    long double a, long double b,
    long double eps, int *iters);

#endif
