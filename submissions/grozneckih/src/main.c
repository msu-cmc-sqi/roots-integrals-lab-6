#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

long double f1(long double x);
long double f2(long double x);
long double f3(long double x);

long double df1(long double x);
long double df2(long double x);
long double df3(long double x);

//передача f1, f2 и f3 и их производных в root и integral
typedef long double (*func_t)(long double);

//разность f(x)-g(x), корень из разности равен точке пересечения графиков f и g
long double raznost(func_t f, func_t g, long double x)
{
    return f(x) - g(x);
}
//разность f'(x)-g'(x) для метода Ньютона
long double raznostproisv(func_t df, func_t dg, long double x)
{
    return df(x) - dg(x);
}

//поиск точки пересения двух функций на отрезке
long double root(func_t f, func_t g, func_t df, func_t dg, long double a, long double b, long double eps, int *iterations)
{
    long double x_hord;
    long double x_newton;
    long double fa;
    long double fb;
    long double fx;
    long double dfx;

    x_hord = a;
    x_newton = b;
    *iterations = 0;

    do {
        fa = raznost(f, g, a);
        fb = raznost(f, g, b);
        //приближение методом хорд
        x_hord = (a * fb - b * fa) / (fb - fa);
        //приближение методом Ньютона
        fx = raznost(f, g, x_newton);
        dfx = raznostproisv(df, dg, x_newton);

        x_newton = x_newton - fx / dfx;

        if (raznost(f, g, x_hord) * raznost(f, g, a) < 0)
            b = x_hord;
        else
            a = x_hord;

        (*iterations)++;

    } while (fabsl(x_hord - x_newton) > eps);

    return (x_hord + x_newton) / 2.0L;
}

//вычисление определенного интеграла методом трапеций
long double integral(func_t f, long double a, long double b, long double eps)
{
    int n;
    int i;
    long double h;
    long double sum;
    long double prev;
    long double cur;
    long double x;

    n = 4;
    prev = 0.0L;

    do {
        h = (b - a) / n;
        sum = (f(a) + f(b)) / 2.0L;

        for (i = 1; i < n; i++) {
            x = a + i * h;
            sum += f(x);
        }

        cur = sum * h;

        if (fabsl(cur - prev) < eps)
            break;

        prev = cur;
        n *= 2;

    } while (1);

    return cur;
}

func_t get_func(char *name)
{
    if (strcmp(name, "f1") == 0)
        return f1;
    if (strcmp(name, "f2") == 0)
        return f2;
    if (strcmp(name, "f3") == 0)
        return f3;

    return NULL;
}

func_t get_dfunc(char *name)
{
    if (strcmp(name, "f1") == 0)
        return df1;
    if (strcmp(name, "f2") == 0)
        return df2;
    if (strcmp(name, "f3") == 0)
        return df3;

    return NULL;
}

void print_help(void)
{
    printf("Usage:\n");
    printf("./program\n");
    printf("./program --help\n");
    printf("./program --roots\n");
    printf("./program --iterations\n");
    printf("./program --test-root f1 f2 a b eps\n");
    printf("./program --test-integral f1 a b eps\n");
}

//основной расчет, нахождение точек пересечения, вычисление площади
void calculate(long double eps, long double *x13, long double *x23, long double *x12, long double *s, int *it13, int *it23, int *it12)
{
    *x13 = root(f1, f3, df1, df3, 1.0L, 2.0L, eps, it13);
    *x23 = root(f2, f3, df2, df3, 4.0L, 5.0L, eps, it23);
    *x12 = root(f1, f2, df1, df2, 5.0L, 6.0L, eps, it12);

    *s = fabsl(integral(f3, *x13, *x23, eps) - integral(f1, *x13, *x23, eps) + integral(f2, *x23, *x12, eps) - integral(f1, *x23, *x12, eps));
}

//печать результата
void print_result(long double eps)
{
    long double x13;
    long double x23;
    long double x12;
    long double s;

    int it13;
    int it23;
    int it12;

    calculate(eps, &x13, &x23, &x12, &s, &it13, &it23, &it12);

    printf("eps = %Lf\n", eps);

    printf("x13 = %Lf\n", x13);
    printf("iterations = %d\n", it13);

    printf("x23 = %Lf\n", x23);
    printf("iterations = %d\n", it23);

    printf("x12 = %Lf\n", x12);
    printf("iterations = %d\n", it12);

    printf("S = %Lf\n\n", s);
}

//печать найденных точек пересечения графиков
void print_roots(void)
{
    long double x13;
    long double x23;
    long double x12;
    long double s;

    int it13;
    int it23;
    int it12;

    calculate(1e-7L, &x13, &x23, &x12, &s, &it13, &it23, &it12);

    printf("x13 = %Lf\n",x13);
    printf("x23 = %Lf\n", x23);
    printf("x12 = %Lf\n", x12);
}

//печать кол-ва итераций для каждого корня
void print_iterations(void)
{
    long double x13;
    long double x23;
    long double x12;
    long double s;

    int it13;
    int it23;
    int it12;

    calculate(1e-7L, &x13, &x23, &x12, &s, &it13, &it23, &it12);

    printf("iterations x13 = %d\n", it13);
    printf("iterations x23 = %d\n", it23);
    printf("iterations x12 = %d\n", it12);
}

int main(int argc, char *argv[])
{
    func_t f;
    func_t g;
    func_t df;
    func_t dg;

    long double a;
    long double b;
    long double eps;
    long double res;

    int iterations;

    if (argc == 1) {
        print_result(1e-3L);
        print_result(1e-5L);
        print_result(1e-7L);
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }

    if (strcmp(argv[1], "--roots") == 0) {
        print_roots();
        return 0;
    }

    if (strcmp(argv[1], "--iterations") == 0) {
        print_iterations();
        return 0;
    }

    if (strcmp(argv[1], "--test-root") == 0) {
        if (argc != 7) {
            print_help();
            return 1;
        }

        f = get_func(argv[2]);
        g = get_func(argv[3]);
        df = get_dfunc(argv[2]);
        dg = get_dfunc(argv[3]);

        if (f == NULL || g == NULL || df == NULL || dg == NULL) {
            printf("Wrong function name\n");
            return 1;
        }

        a = strtold(argv[4], NULL);
        b = strtold(argv[5], NULL);
        eps = strtold(argv[6], NULL);

        res = root(f, g, df, dg, a, b, eps, &iterations);

        printf("root = %Lf\n", res);
        printf("iterations = %d\n", iterations);

        return 0;
    }

    if (strcmp(argv[1], "--test-integral") == 0) {
        if (argc != 6) {
            print_help();
            return 1;
        }

        f = get_func(argv[2]);

        if (f == NULL) {
            printf("Wrong function name\n");
            return 1;
        }

        a = strtold(argv[3], NULL);
        b = strtold(argv[4], NULL);
        eps = strtold(argv[5], NULL);

        res = integral(f, a, b, eps);

        printf("integral = %Lf\n", res);

        return 0;
    }

    print_help();

    return 0;
}