#include "Stack.h"
#include <stdbool.h>

#define STACK_EXIST(s) { if (s == NULL) exit(-1)}

Stack *create_stack()
{
  Stack *s = malloc(sizeof(Stack));
  s->size = 0;

  return s;
}

// Uses a void pointer to allow any type
int stack_push(Stack *s, void *content)
{
  if (s == NULL)
    return -1;
  s->content[s->size] = content;
  s->size += 1;

  return 0;
}

void *stack_pop(Stack *s)
{
  if (s == NULL)
    exit(-1);

  void *value;
  value = s->content[(s->size)-1];
  s->content[(s->size)-1] = NULL;

  if (stack_is_empty(s))
    exit(-1);
  s->size -= 1;

  return value;
}

bool stack_is_empty(Stack *s)
{
  if (s == NULL)
    exit(-1);

  if (s->size == 0)
    return true;
  else
    return false;

}
