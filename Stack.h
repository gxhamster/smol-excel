#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdbool.h>

#define STACK_MAX 200
// Stack can only contain one types of elements
typedef struct {
  void *content[STACK_MAX];
  size_t size;
} Stack;


Stack *create_stack();
int stack_push(Stack *s, void *content);
void *stack_pop(Stack *s);
bool stack_is_empty(Stack *s);

#endif
