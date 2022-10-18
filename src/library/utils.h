#ifndef UTILS
#define UTILS

#include <stdio.h>

void rand_points(const int n, const int d, const double a, double* points);

void read_points(const int n, const int d, FILE* file, double* points);

void print_points(const int n, const int d, const double* points);

void get_distances(const int n, const int d, const int vp, const double* points, const int* idx, double* distances);

double calculate_distance(const int d, const int idx1, const int idx2, const double* points);

double get_median_distance(const int n, double* distances);

void merge(double* arr, int l, int m, int r);

void mergeSort(double* arr, int l, int r);

#endif