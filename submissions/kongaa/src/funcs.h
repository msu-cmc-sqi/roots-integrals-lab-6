#ifndef FUNCS_H
#define FUNCS_H

extern int root_iterations;
extern int integral_iterations;

double f1(double x);
double f2(double x);
double f3(double x);

double root(
    double (*f)(double),
    double (*g)(double),
    double a,
    double b,
    double eps
);

double integral(
    double (*f)(double),
    double a,
    double b,
    double eps
);

#endif
