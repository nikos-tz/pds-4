#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "library/tree.h"
#include "library/utils.h"
#include "threshold_parallel/vptree.h"

int main(int argc, char *argv[]){

  if(argc < 4){
    printf("Enter number of points, dimension size and threshold\n");
    exit(1);
  }

  const int n = atoi(argv[1]);
  if(n < 1){
    printf("Enter positive number of points\n");
    exit(1);
  }
  const int d = atoi(argv[2]);
  if(d < 1){
    printf("Enter positive dimension size\n");
    exit(1);
  }
  const int threshold = atoi(argv[3]);
  printf("threshold: %d\n", threshold);
  if(threshold < 1){
    printf("Enter positive threshold\n");
    exit(1);
  }
  const double a = 1.0;

  printf("For n = %d, d = %d\n", n, d);

  double* points = (double*) malloc(n * d * sizeof(double));

  if(argc > 4){
    const char* input_filename = argv[4];
    FILE* input_file = fopen(input_filename, "r");
    read_points(n, d, input_file, points);
    fclose(input_file);
  }
  else{
    srand((unsigned int)time(NULL));
    rand_points(n, d, a, points);
  }

  double time = 0.0;
  struct timeval start,end;
  gettimeofday(&start,NULL);

  struct T* tree = threshold_parallel_vp_tree(n,d,threshold,points);

  gettimeofday(&end,NULL);
  time = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
  printf("parallel time: %f sec\n", time);

  if(argc > 5){
    const char* output_filename = argv[5];
    FILE* output_file = fopen(output_filename, "w");
    save_tree(tree, output_file);
    fclose(output_file);
  }

  delete_tree(tree);
  free(points);

  return 0;
}