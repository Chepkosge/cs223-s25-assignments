#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  // TODO: Implement malloc with a free list (See reading for details)
  if(size <= 0){
    return NULL;
  }

  struct chunk* ptr = flist;
  struct chunk* prev = NULL;
  
  while((ptr != NULL) && (ptr->size < (size + sizeof(struct chunk)))){
    prev = ptr;
    ptr = ptr->next;
  }
  if((ptr != NULL) && (ptr->size >= (size + sizeof(struct chunk)))){
    if(prev != NULL){
      prev->next = ptr->next;
    }
    else{
      flist = flist->next;
    }
    ptr->next = NULL;
    ptr->used = size;
    return (void*) (ptr + 1);
  }
  else{
    ptr = sbrk(size + sizeof(struct chunk));
    ptr->size = size;
    ptr->used = size;
    ptr->next = NULL;
    return (void*) (ptr + 1);
  }
  return NULL;
}

void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  if(memory != NULL){
    struct chunk* curr = flist;
    flist = (struct chunk*) memory - 1;
    flist -> used = 0;
    flist->next = curr;
    return;
  }
  return;
}
