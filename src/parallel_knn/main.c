#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "/usr/include/mpi/mpi.h"


#include "parallel_knn/knn.h"
#include "library/tree.h"
#include "library/utils.h"
#include "sequential/vptree.h"

int main(int argc, char *argv[]){

  if(argc < 4){
    printf("Enter number of points, dimension size and number of neighbors\n");
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
  const int k = atoi(argv[3]);
  if(k < 1){
    printf("Enter positive number of neighbors\n");
    exit(1);
  }
  const double a = 1.0;

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

  // int* idx = (int*) malloc(n * sizeof(int));
  // for(int i=0; i<n; i++){
  //   idx[i] = i;
  // }
  // double* distances = (double*) malloc((n-1) * sizeof(double));
  // get_distances(n, d, n-1, points, idx, distances);
  // for(int i=0; i<(n-1); i++){
  //   printf("distance[] = %lf\n", distances[i]);
  // }

  struct T* tree = sequential_vp_tree(n,d,points);

  struct Neighbors* data = (struct Neighbors*) malloc(n * sizeof(struct Neighbors));
  struct Neighbors** neighbors = (struct Neighbors**) malloc(n * sizeof(struct Neighbors*));

  for(int i=0; i<n; i++){
    neighbors[i] = &data[i];
  }

  int rank;
  int nproc;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  if(rank==0){
    printf("For n = %d, d = %d, k = %d\n", n, d, k);
  }


  int* n_all = (int*) malloc(nproc * sizeof(int));
  for(int i=0; i<nproc; i++){
    n_all[i] = n/nproc;
    if(i< (n%nproc)){
      n_all[i]++;
    }
  }

  int* prefix_n_all = (int*) calloc(nproc+1, sizeof(int));
  for(int i=0; i<nproc; i++){
    prefix_n_all[i+1] = prefix_n_all[i] + n_all[i];
  }

  double time = 0.0;
  struct timeval start,end;
  gettimeofday(&start,NULL);

  parallel_knn(tree, points, prefix_n_all, rank, k, neighbors);

  MPI_Barrier(MPI_COMM_WORLD);
  gettimeofday(&end,NULL);
  time = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
  if(rank==0){
    printf("knn parallel time: %f sec\n", time);
  }

  delete_neighbors(n, neighbors);
  delete_tree(tree);
  free(points);

  MPI_Finalize();

  return 0;
}

// #include <stdio.h>
// #include <string.h>
// #include "/usr/include/mpi/mpi.h"

// int main(int argc, char** argv){
//   int pid;
//   int nproc;

//   char message[100];
//   MPI_Status status;
//   int tag = 50;

//   MPI_Init(&argc, &argv);
//   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
//   MPI_Comm_size(MPI_COMM_WORLD, &nproc);

//   if (pid != 0) {
//     sprintf(message, "Hello from process %d!", pid);

//     printf("%s\n",message);

//     int dest = 0;
//     MPI_Send(message, strlen (message)+1, MPI_CHAR, dest,
// 	     tag, MPI_COMM_WORLD);
//   } else {
    
//     int k = 1;
//     while (k < nproc) {
      
//       MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, tag,
// 	       MPI_COMM_WORLD, &status);
      
//       printf("Received >>%s<<\n", message);
 
//       k++;
//     }
//   }
//   MPI_Finalize();

//   return(0);
// }
