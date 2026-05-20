#include <stdio.h>
#include <string.h>
#include <math.h>

typedef float (*func_t)(float);

float f1(float), f2(float), f3(float);

int root_chord(func_t, func_t, float, float, float, int*, float*);

int root_combined(func_t, func_t, float, float, float, int*, float*);

float integral_simpson(func_t, float, float, float);

static float diff12(float x) {return f1(x)-f2(x);} 

static float diff13(float x) {return f1(x)-f3(x);} 
static float diff23(float x) {return f2(x)-f3(x);} 

static int bracket(func_t d, float *a, float *b) 
{
    float l = -10.0f;
    float step = 0.05f;
    float prev = d(l);
    for (float r = l + step; r <= 10.0f; r += step) 
    {
        float cur = d(r);
        if (prev * cur <= 0) 
        { 
            *a = r - step; *b = r; return 0; 
        }
        prev = cur;
    }
    return -1;
}

int main(int argc, char **argv) 
{
    float eps = 1e-3f;
    float a12, b12, a13, b13, a23, b23;
    if (bracket(diff12, &a12, &b12) || bracket(diff13, &a13, &b13) || bracket(diff23, &a23, &b23)) 
    {
        fprintf(stderr,"failed to bracket roots\n");
        return 1;
    }

    float x12, x13, x23;
    int i12, i13, i23;
    root_chord(f1, f2, a12, b12, eps, &i12, &x12);
    root_combined(f1, f3, a13, b13, eps, &i13, &x13);
    root_chord(f2, f3, a23, b23, eps, &i23, &x23);

    if (argc > 1 && strcmp(argv[1],"--roots") == 0) 
    {
        printf("x12=%.6f\nx13=%.6f\nx23=%.6f\n", x12, x13, x23);
        return 0;
    }
    if (argc > 1 && strcmp(argv[1],"--iterations") == 0) 
    {
        printf("iters12=%d\niters13=%d\niters23=%d\n", i12, i13, i23);
        return 0;
    }
    if (argc > 1 && strcmp(argv[1],"--help") == 0) 
    {
        puts("--help --roots --iterations");
        return 0;
    }

    float l = x13, m = x23, r = x12;
    if (!(l < m && m < r)) {
        float arr[3] = {x12, x13, x23};
        for(int i = 0; i < 3; i++)
        {
            for(int j = i + 1; j < 3; j++)
            {
                if(arr[i] > arr[j]) 
                {
                    float t = arr[i];
                    arr[i] = arr[j];
                    arr[j] = t;
                }
            }
        }
        l=arr[0]; 
        m=arr[1]; 
        r=arr[2];
    }

    float s = integral_simpson(diff13, l, m, 1e-6f) + integral_simpson(diff12, m, r, 1e-6f);
    
    printf("Area ~= %.6f\n", fabsf(s));
    return 0;
}
