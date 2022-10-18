#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "knn/knn.h"
#include "library/tree.h"
#include "library/utils.h"

void print_neighbors(struct Neighbors* neighbors){
  for(int i=0; i<neighbors->num_neighbors; i++){
    printf("point: %d, distance = %lf\n", neighbors->idx[i], neighbors->distances[i]);
  }
}

void delete_neighbors(const int n, struct Neighbors** neighbors){
  for(int i=0; i<n; i++){
    free(neighbors[i]->distances);
    free(neighbors[i]->idx);
  }
  free(neighbors[0]);
  free(neighbors);
}

void add_neighbor(const double distance, const int idx, const int k, struct Neighbors* neighbors){
  if(neighbors->num_neighbors == 0){
    neighbors->distances[0] = distance;
    neighbors->idx[0] = idx;
    neighbors->num_neighbors++;
  }
  else{
    int new_idx = 0;
    for(int i=neighbors->num_neighbors-1; i>=0; i--){
      if(distance > neighbors->distances[i]){
        new_idx = i+1;
        break;
      }
    }
    for(int i=neighbors->num_neighbors; i>new_idx; i--){
      neighbors->distances[i] = neighbors->distances[i-1];
      neighbors->idx[i] = neighbors->idx[i-1];
    }
    neighbors->distances[new_idx] = distance;
    neighbors->idx[new_idx] = idx;
    if(neighbors->num_neighbors < k){
      neighbors->num_neighbors++;
    }
  }
}

void calculate_knn(const struct T* tree, const double* points, const int k, const int q, struct Neighbors* neighbors){

  if(tree){
    double distance = calculate_distance(tree->d, tree->idx, q, points);

    if((tree->idx != q) && (distance < neighbors->tau)){
      add_neighbor(distance, tree->idx, k, neighbors);
      if(neighbors->num_neighbors == k){
        neighbors->tau = neighbors->distances[k-1];
      }
    }

    if(distance < tree->md){
      if(distance < (tree->md + neighbors->tau)){
        calculate_knn(tree->inner, points, k, q, neighbors);
      }
      if(distance >= (tree->md - neighbors->tau)){
        calculate_knn(tree->outer, points, k, q, neighbors);
      }
    }
    else{
      if(distance >= (tree->md - neighbors->tau)){
        calculate_knn(tree->outer, points, k, q, neighbors);
      }
      if(distance < (tree->md + neighbors->tau)){
        calculate_knn(tree->inner, points, k, q, neighbors);
      }
    }
  }

}

void parallel_knn(const struct T* tree, const double* points, const int* prefix_n_all, const int rank, const int k, struct Neighbors** neighbors){

  for(int i=prefix_n_all[rank]; i<prefix_n_all[rank+1]; i++){
    neighbors[i]->num_neighbors = 0;
    neighbors[i]->tau = DBL_MAX;
    neighbors[i]->distances = (double*) malloc(k * sizeof(double));
    neighbors[i]->idx = (int*) malloc(k * sizeof(int));

    calculate_knn(tree, points, k, i, neighbors[i]);
  }

}