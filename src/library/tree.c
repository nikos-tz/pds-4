#include <stdlib.h>
#include <stdio.h>

#include "library/tree.h"
#include "library/utils.h"

void delete_tree(struct T* tree){
  if (tree) {
    delete_tree(tree->inner);
    delete_tree(tree->outer);
    free(tree);
  }
}

void save_tree(const struct T* tree, FILE* file){
  if(tree){
    for(int i=0; i<tree->d; i++){
      fprintf(file, "%lf\n", tree->vp[i]);
    }
    save_tree(tree->inner, file);
    save_tree(tree->outer, file);
  }
}
