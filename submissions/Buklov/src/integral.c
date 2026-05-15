#include <math.h>
#include "../include/funcs.h"

/* интегрирование методом прямоугольников (средние точки).
   удваиваем число разбиений, пока правило Рунге не даст погрешность < eps */

/* Правило Рунге — это практический способ оценки погрешности
   численных методов (короче когда мы для проверки точности уменьшаем шаг)*/
double integral(double (*f)(double), double a, double b, double eps) {
    int n = 1;
    double h = b - a;
    double I_n = f(a + h / 2) * h;

    while (1) {
        n *= 2;
        h = (b - a) / n;

        double I_2n = 0;
        for (int i = 0; i < n; i++) {
            I_2n += f(a + (i + 0.5) * h);
        }
        I_2n *= h;

        if (fabs(I_2n - I_n) / 3 < eps) return I_2n;
        I_n = I_2n;
    }
}
