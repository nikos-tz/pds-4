#ifndef VPTREE
#define VPTREE

#include <stdio.h>

struct T* parallel_vpt(const int n, const int d, double* points, const int* idx);

struct T* parallel_vp_tree(const int n, const int d, double* points);

#endif