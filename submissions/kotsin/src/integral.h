#ifndef INTEGRAL_H
#define INTEGRAL_H

/* Считает определённый интеграл f на [a, b] с точностью eps
   Метод - Симпсон с правилом Рунге */
long double integral(
    long double (*f)(long double),
    long double a, long double b,
    long double eps);

#endif
