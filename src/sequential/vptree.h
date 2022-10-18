#ifndef VPTREE
#define VPTREE

struct T* sequential_vpt(const int n, const int d, double* points, const int* idx);

struct T* sequential_vp_tree(const int n, const int d, double* points);

#endif