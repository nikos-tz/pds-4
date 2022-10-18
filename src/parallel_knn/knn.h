#ifndef KNN
#define KNN

#include "library/tree.h"

struct Neighbors{
  int num_neighbors;
  double tau;
  double* distances;
  int* idx;
};

void print_neighbors(struct Neighbors* neighbors);

void delete_neighbors(const int n, struct Neighbors** neighbors);

void add_neighbor(const double distance, const int idx, const int k, struct Neighbors* neighbors);

void calculate_knn(const struct T* tree, const double* points, const int k, const int q, struct Neighbors* neighbors);

void parallel_knn(const struct T* tree, const double* points, const int* prefix_n_all, const int rank, const int k, struct Neighbors** neighbors);

#endif