#ifndef FUNCS_H
#define FUNCS_H

/* функции варианта (NASM) */
extern double f1(double x);
extern double f2(double x);
extern double f3(double x);

/* корень уравнения f(x) = g(x) на [a, b] */
double root(double (*f)(double), double (*g)(double),
            double a, double b, double eps);

/* интеграл от f на [a, b] */
double integral(double (*f)(double), double a, double b, double eps);

#endif
