#include <cilk/cilk.h>
#include <stdlib.h>
#include <stdio.h>

#include "library/utils.h"

void rand_points(const int n, const int d, const double a, double* points){
  for(int i=0; i < n*d; i++){
    points[i] = ( (double)rand() / (double)(RAND_MAX) ) * a;
  }
}

void read_points(const int n, const int d, FILE* file, double* points){
  for(int i=0; i < n*d; i++){
    if(fscanf(file, "%lf\n", &points[i]) != 1){
      printf("failed to read points\n");
      exit(1);
    }
  }
}

void print_points(const int n, const int d, const double* points){
  for(int i=0; i < n; i++){
    for(int j=0; j < d; j++){
      printf("%lf ", points[i*d + j]);
    }
    printf("\n");
  }
}

void get_distances(const int n, const int d, const int vp, const double* points, const int* idx, double* distances){
  for(int i=0; i<(n-1); i++){
    for(int j=0; j<d; j++){
      distances[i] += (points[vp*d + j] - points[idx[i]*d + j]) * (points[vp*d + j] - points[idx[i]*d + j]);
    }
  }
}

double calculate_distance(const int d, const int idx1, const int idx2, const double* points){
  double distance = 0.0;
  for(int j=0; j<d; j++){
    distance += (points[idx1*d + j] - points[idx2*d + j]) * (points[idx1*d + j] - points[idx2*d + j]);
  }
  return distance;
}

double get_median_distance(const int n, double* distances){
  mergeSort(distances, 0, n-1);
  int median_id = n/2;
  if(n%2 == 0){
    return (distances[median_id] + distances[median_id-1]) / 2.0;
  }
  return distances[median_id];
}

void merge(double* arr, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  double L[n1], R[n2];

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    }
    else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
  are any */
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  /* Copy the remaining elements of R[], if there
  are any */
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(double* arr, int l, int r)
{
  if (l < r) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int m = l + (r - l) / 2;

    // Sort first and second halves
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    merge(arr, l, m, r);
  }
}