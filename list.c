#include <stdio.h>
#include <stdlib.h>

struct List {
  struct List *next;
  int data;
};

void push(struct List **list, int data) {
  struct List *old = *list;
  *list = malloc(sizeof(struct List));
  (*list)->data = data;
  (*list)->next = old;
}

int pop(struct List **list, int *data) {
  if (!(*list)->next) return 0;
  *data = (*list)->data;
  *list = (*list)->next;
}

int list_len(struct List *list) {
  struct List *iter;
  int i = 0;
  for (iter = list; iter->next; iter = iter->next) { i++; }
  return i;
}

int main(void) {
  struct List *handle = malloc(sizeof(struct List));
  handle->next = 0;

  int i;
  for (i=0; i<10; ++i) {
    printf("pushing: %d\n", i);
    push(&handle, i);
  }

  printf("list has %d elements\n", list_len(handle));

  for (i=0; i<10; ++i) {
    int data;
    pop(&handle, &data);
    printf("popping: %d\n", data);
  }

  printf("list has %d elements\n", list_len(handle));

  return 1;
}
