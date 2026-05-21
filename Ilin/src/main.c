#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funcs.h"
#include "root.h"
#include "integral.h"
#define EPS 1e-4

void print_help() {
printf("Usage ./lab6 [OPTIONS]\n");
printf(" --help show this help\n");
printf(" --roots printf intersection points\n");
printf(" --iterations print number of iterations\n");
printf(" --test-root test root finding\n");
printf(" --test-integral test integral computaton\n");
}
void test_root(long double eps){
printf("\n=== Testing root finding ===\n");
printf("Equation: x^2 - 2 =0 (root = sqrt(2) = 1.41421356)\n");

long double f_sq(long double x) {return x*x;}
long double g_sq(long double x) {return 2.0;}
long double df_sq(long double x) {return 2.0 * x;}
long double dg_sq(long double x) {return 0.0;}
int iters;
#ifdef USE_NEWTON
	long double root = newton_root(f_sq, g_sq, df_sq, dg_sq, 1.0, 2.0, eps, &iters);
	printf("Newton: root = %.10Lf, iterations = %d\n", root, iters);
#else
	long double root = bisection_root(f_sq, g_sq, 1.0, 2.0, eps, &iters);
	printf("Bisection: root = %.10Lf, iterations = %d\n", root, iters);
#endif
printf("Newton: root = %.10Lf, iterations = %d\n", root, iters);
}
void test_integral(long double eps){
printf("\n=== Testing integral computation===\n");
printf("Function: f(x) = x (integral from 0 to 1 = 0.5)\n");
long double f_lin(long double x) {return x;}
int iters;
long double result = integral(f_lin, 0.0, 1.0, eps, &iters);
printf("Integral: result = %.10Lf, expected  = 0.5, iterations = %d\n", result, iters);
}
int main(int argc, char **argv){
int show_roots = 0, show_iters = 0, test_root_flag = 0, test_integral_flag  =0;
long double eps = EPS;
for (int i =1;i<argc; i++){
if (strcmp(argv[i], "--help") == 0){
print_help();
return 0;
} else if (strcmp(argv[i], "--roots") == 0){
show_roots = 1;
} else if(strcmp(argv[i], "--iterations") == 0) {
show_iters = 1;
} else if(strcmp(argv[i], "--test-root") == 0){
test_root_flag =1;
} else if(strcmp(argv[i], "--test-integral") == 0){
test_integral_flag = 1;
}
}
if (test_root_flag){
test_root(eps);
return 0;
}
if (test_integral_flag){
test_integral(eps);
return 0;
}
int iters;
long double x12 = bisection_root(f1, f2, 0.4, 0.5, eps, &iters);
if (show_iters) printf("Iterations for f1 = f2: %d\n", iters);
long double x23 = bisection_root(f2,f3, 0.45, 0.55, eps, &iters);
if (show_iters) printf("Iterations for f2 = f3: %d\n", iters);
long double x13 = bisection_root(f1,f3, 0.35, 0.4, eps, &iters);
if (show_iters) printf("Iterations for f1 = f3: %d\n", iters);
if (show_iters) {
	printf("\n=== Intersection points ===\n");
	printf("f1 = f3: x = %.10Lf\n", x13);
	printf("f1 = f2: x = %.10Lf\n", x12);
	printf("f2 = f3: x = %.10Lf\n", x23);
}
long double points[3] = {x13, x12, x23};
for (int i = 0; i < 2; i++){
	for (int j =i+1; j<3; j++){
		if(points[i] > points[j]){
			long double t = points[i];
			points[i] = points[j];
			points[j] = t;
		}
	}
}

long double area_part1 =  integral(f2, points[0], points[1], eps, &iters) - integral(f1, points[0], points[1], eps, &iters);
long double area_part2 = integral(f3, points[1], points[2], eps, &iters) - integral(f1, points[1], points[2], eps, &iters);
long double total = area_part1 + area_part2;
printf("\n=== Area ===\n");
printf("Area 1 (f1-f2) on [%.6Lf, %.6Lf] = %.10Lf\n", points[0], points[1], area_part1);
printf("Area 2 (f1 - f3) on [%.6Lf, %.6Lf] = %.10Lf\n", points[1], points[2], area_part2);
printf("Total %0.10Lf\n", total);
return 0;
}

