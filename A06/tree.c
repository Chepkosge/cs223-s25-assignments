#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node* find(const char* name, struct tree_node* root){
  if (root == NULL) {
    return NULL;  
  }

  int cmp = strcmp(name, root->data.name);

  if (cmp == 0) {
      return root; 
  } else if (cmp < 0) {
      return find(name, root->left); 
  } else {
      return find(name, root->right); 
  }
  return NULL;
}

struct tree_node* insert(const char* name, struct tree_node* root){
  struct tree_node* new = (struct tree_node*)malloc(sizeof(struct tree_node));
  if (new == NULL) {
      printf("Memory allocation error\n");
      return NULL;
  }
  strcpy(new->data.name, name);
  new->left = new->right = NULL;

  if (root == NULL) {
      return new;
  }

  struct tree_node* current = root;
  struct tree_node* parent = NULL;

  while (current != NULL) {
      parent = current;
      if (strcmp(name, current->data.name) < 0) {
          current = current->left;
      } 
      else if (strcmp(name, current->data.name) > 0) {
          current = current->right;
      } 
      else {
          free(new);
          return root;
      }
  }

  if (strcmp(name, parent->data.name) < 0) {
    parent->left = new;
  } 
  else {
    parent->right = new;
  }
  
  return root;
}


void clear(struct tree_node* root) {
    if (root == NULL) return;

    struct tree_node* stack[1024];
    int top = -1;
    stack[++top] = root;

    while (top >= 0) {
        struct tree_node* current = stack[top--];

        if (current->left) stack[++top] = current->left;
        if (current->right) stack[++top] = current->right;

        free(current);
    }
}


void print(struct tree_node* root){
  if (root == NULL) return;

  struct tree_node* stack[1024];  // Using a stack to avoid recursion
  int top = -1;
  stack[++top] = root;

  while (top >= 0) {
      struct tree_node* current = stack[top--];

      if (current->left) stack[++top] = current->left;
      if (current->right) stack[++top] = current->right;

      printf("%s\n", current->data.name);
  }
}

void printSorted(struct tree_node* root){
    struct tree_node* stack[1024];
    int top = -1;
    struct tree_node* current = root;

    while (current != NULL || top >= 0) {
        while (current != NULL) {
            stack[++top] = current;
            current = current->right;
        }
        current = stack[top--];
        printf("%s\n", current->data.name);
        current = current->left;
    }
}

