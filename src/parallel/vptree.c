#include <cilk/cilk.h>
#include <stdlib.h>
#include <stdio.h>

#include "library/tree.h"
#include "library/utils.h"
#include "parallel/utils.h"
#include "parallel/vptree.h"

struct T* parallel_vpt(const int n, const int d, double* points, const int* idx){
  if(n==0){
    return NULL;
  }

  struct T* tree = (struct T*) malloc(sizeof(struct T));

  tree->n = n;
  tree->d = d;
  tree->vp = &(points[idx[n-1]*d]);
  tree->idx = idx[n-1];

  if(n==1){
    tree->md = 0.0;
    tree->inner = NULL;
    tree->outer = NULL;
    return tree;
  }

  double* distances = (double*) malloc((n-1) * sizeof(double));
  parallel_get_distances(n, d, tree->idx, points, idx, distances);
  tree->md = get_median_distance(n-1, distances);

  int inner_size = 0;
  int* idx_inner = (int*) malloc(n * sizeof(int));
  int outer_size = 0;
  int* idx_outer = (int*) malloc(n * sizeof(int));

  for(int i=0; i<(n-1); i++){
    double distance = 0.0;
    for(int j=0; j<d; j++){
      distance += (points[tree->idx*d + j] - points[idx[i]*d + j]) * (points[tree->idx*d + j] - points[idx[i]*d + j]);
    }
    if(distance <= tree->md){
      idx_inner[inner_size++] = idx[i];
    }
    else{
      idx_outer[outer_size++] = idx[i];
    }
  }


  if(inner_size == 0){
    tree->inner = NULL;
  }
  else{
    tree->inner = cilk_spawn parallel_vpt(inner_size, d, points, idx_inner);
  }

  if(outer_size == 0){
    tree->outer = NULL;
  }
  else{
    tree->outer = parallel_vpt(outer_size, d, points, idx_outer);
  }

  cilk_sync;
  free(distances);
  free(idx_inner);
  free(idx_outer);

  return tree;
}

struct T* parallel_vp_tree(const int n, const int d, double* points){
  int* idx = (int*) malloc(n * sizeof(int));
  for(int i=0; i<n; i++){
    idx[i] = i;
  }
  struct T* tree = parallel_vpt(n, d, points, idx);
  free(idx);
  return tree;
}
