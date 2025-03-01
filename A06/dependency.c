#include <stdio.h>
#include "tree.h"

int main(int argc, char* argv[]){
  struct tree_node* root = malloc(sizeof(struct tree_node));
    if (root == NULL) {
        printf("Memory allocation error\n");
        return 1;
    }
    
    strcpy(root->data.name, argv[1]);
    root->left = root->right = NULL;

    for (int i = 2; i < argc; i++) {
        insert(argv[i], root);
    }

    printf("Processing %d files...\n", argc);
    char command[20];
    scanf("%s", command);

    if(strcmp(command, "list") == 0){
      printSorted(root);
      
    }
    
    else if(strcmp(command, "quit") == 0){
      clear(root);
      return 0;
    }

    else{
      if(find(command, root) == NULL){
        printf("%s not found\n", command);
      }
      else{
        FILE* file = fopen(command, "r");
        if (file == NULL) {
          printf("Failed to open file %s\n", command);
          return 1;
        }
        char buffer[1024];
        while(fgets(buffer, 1024, file) != NULL){
          printf("%s has the following dependencies\n", command);
          if(strncmp(buffer, "#include", 7) == 0){
            for(int i=9; i<strlen(buffer); i++){
              printf("%c", buffer[i]);
            }
            printf("\n");
          }
        }
        fclose(file);
      }
    }

  return 0;
}
