#include "integral.h"
#include <math.h>
long double integral(long double (*f)(long double), long double a,long double b, long double eps, int *iters){
if(a>=b) return 0.0;
int n = 1;
long double h_ = (b-a);
long double S_prev = f((a+b)/2.0)*h_;
long double S_curr;
int max_iter = 100000;
int iter = 0;
do {
n*=2;
h_ = (b-a)/n;
S_curr = 0.0;
for(int i = 0; i<n;i++){
long double x_mid = a+(i+0.5L)*h_;
S_curr += f(x_mid);
}
S_curr *= h_;
iter++;
if (fabsl(S_curr - S_prev) < eps) break;
S_prev = S_curr;
} while (iter < max_iter);
if (iters) *iters = iter;
return S_curr;
}
