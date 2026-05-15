#ifndef NUMERICAL_H
#define NUMERICAL_H

// Тип указателя на функцию f(x)
typedef float (*func_t)(float);

// Метод деления пополам
float root_bisection(func_t f, func_t g, float a, float b, float eps, int *iters);

// Метод Ньютона
float root_newton(func_t f, func_t g, func_t df, func_t dg,
                  float a, float b, float eps, int *iters);

// Метод прямоугольников
float integral_rect(func_t f, float a, float b, float eps, int *iters);

#endif
