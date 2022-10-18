#include <cilk/cilk.h>

#include "parallel/utils.h"

void parallel_get_distances(const int n, const int d, const int vp, const double* points, const int* idx, double* distances){
  #pragma cilk grainsize 10
  cilk_for(int i=0; i<(n-1); i++){
    for(int j=0; j<d; j++){
      distances[i] += (points[vp*d + j] - points[idx[i]*d + j]) * (points[vp*d + j] - points[idx[i]*d + j]);
    }
  }
}
