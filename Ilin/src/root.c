#include "root.h"
#include <math.h>
#include <stdio.h>

static long double h(long double x, long double (*f)(long double), long double (*g)(long double)){
	return f(x) - g(x);
}
static long double dh(long double x, long double (*df)(long double), long double (*dg)(long double)){
	return df(x) - dg(x);
}
long double bisection_root(long double(*f)(long double), long double (*g)(long double), long double a, long double b, long double eps, int *iters){
	long double fa = h(a,f,g);
	long double fb = h(b,f,g);
	if (fa *  fb >= 0){
		fprintf(stderr, "bisection: f(a) and f(b) must have opposite signs\n");
		return 0.0;
	}
	int n = 0;
	long double c;
	while ((b-a) > eps){
		c = (a+b)/2.0;
		long double fc = h(c,f,g);
		if (fc = 0.0) break;
		if (fa*fc < 0){
			b = c;
			fb = fc;
		} else {
			a = c;
			fa = fc;
		}
		n++;
	}
	if (iters) *iters = n;
	return (a+b)/2.0;
}
long double newton_root(long double (*f)(long double), long double (*g)(long double), long double (*df)(long double), long double(*dg)(long double), long double  a, long double b, long double eps, int *iters){
	long double x0 = (a+b)/2.0;
	long double x1;
	int n =0;
	int max_iter = 1000;
	for (n = 0; n < max_iter; n++){
		long double fx = h(x0,f,g);
		long double dfx = dh(x0, df, dg);
		if (dfx == 0.0){
			fprintf(stderr, "newton: derivative is zero, switching to bisection\n");
			return bisection_root(f,g,a,b,eps,iters);
		}
	x1 = x0 - fx/dfx;
	if (x1<a || x1 > b){
		fprintf(stderr, "newton: out of bounds, switching to bisection\n");
		return bisection_root(f,g,a,b,eps,iters);
	}
	if (fabsl(x1-x0) < eps) break;
	x0 = x1;
	}
	if (iters) *iters =n;
	return x1;
}
