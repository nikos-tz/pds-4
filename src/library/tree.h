#ifndef TREE
#define TREE

#include <stdio.h>

struct T{
  int n; // T.n : number of points under this node;
  int d; // T.d : dimension of points
  double* vp; // T.vp : the vantage point
  int idx; // T.idx : the index of the vantage point in the original set
  double md; // T.md : the median distance of the vantage point to the other points
  struct T* inner; // T.inner : vantage-point subtree
  struct T* outer; // T.inner : vantage-point subtree
};

void delete_tree(struct T* tree);

void save_tree(const struct T* tree, FILE* file);

#endif