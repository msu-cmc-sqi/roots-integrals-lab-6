#ifndef ROOT_H
#define ROOT_H

long double bisection_root(long double (*f)(long double), long double (*g)(long double), long double a, long double b, long double eps, int *iters);
long double newton_root(long double (*f)(long double), long double (*g)(long double),long double(*df)(long double),long double(*dg)(long double), long double a, long double b, long double eps, int *iters);
#endif

