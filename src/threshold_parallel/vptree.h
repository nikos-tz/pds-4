#ifndef VPTREE
#define VPTREE

#include <stdio.h>

struct T* threshold_parallel_vpt(const int n, const int d, const int threshold, double* points, const int* idx);

struct T* threshold_parallel_vp_tree(const int n, const int d, const int threshold, double* points);

#endif