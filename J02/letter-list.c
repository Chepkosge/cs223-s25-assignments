#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node {
  char data;
  struct Node* next;
} Node;
// a new node
Node* createNode(char data) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  if (!newNode) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}
// add character to the linked list
void add(Node** head, char data) {
  Node* newNode = createNode(data);
  if (*head == NULL) {
    *head = newNode;
    return;
  }
  Node* temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newNode;
}
// delete a character from the linked list
void deleteChar(Node** head, char target) {
  Node* temp = *head, *prev = NULL;
  while (temp != NULL && temp->data == target) {
    *head = temp->next;
    free(temp);
    temp = *head;
  }
  while (temp != NULL) {
    while (temp != NULL && temp->data != target) {
      prev = temp;
      temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
    temp = prev->next;
  }
}
void printList(Node* head) {
  while (head != NULL) {
    printf("%c", head->data);
    head = head->next;
  }
  printf("\n");
}
void freeList(Node* head) {
  Node* temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}
int main() {
  Node* head = NULL;
  char word[31], ch;
  printf("Enter a word: ");
  scanf("%30s", word);
  for (int i = 0; i < strlen(word); i++) {
    add(&head, word[i]);
  }
  printf("Original word: ");
  printList(head);
  printf("Enter a character to delete: ");
  scanf(" %c", &ch);
  deleteChar(&head, ch);
  printf("Updated word: ");
  printList(head);
  freeList(head);
  return 0;
}






