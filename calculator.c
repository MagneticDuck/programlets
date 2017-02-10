#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

enum OpType {
  ADD, SUB, MULT, MEM, CONST, ERROR, TERM, CLR
};

struct StackOp { 
  enum OpType type; 
  bool edge;
  int data; 
};

void read_op(struct StackOp *op, char *input) {
  op->edge = false;

  if (strlen(input) == 0) {
    goto error; 
  }

  switch (input[0]) {
    case 'q': {
      op->edge = true;
      op->type = TERM;
      return;
    }
    case '+': { op->type = ADD; return; }
    case '-': { op->type = SUB; return; }
    case '*': { op->type = MULT; return; }
    case 'c': { op->type = CLR; return; }
    case 'm': { 
      int memory;
      if (strlen(input) == 1) goto error;
      if (sscanf(input+1, "%d", &memory)) goto error;
      printf("%d", memory);
      op->type = MEM;
      op->data = memory;
      return; 
    }
    default: {
      int c;
      if (!sscanf(input, "%d", &c)) goto error;
      op->type = CONST;
      op->data = c;
      return;
    }
  }

  error:
    op->edge = true;
    op->type = ERROR;
}

struct Stack {
  int data[127];
  int head;
};

#define STACK_HEAD(stack)  (stack)->data[(stack)->head - 1]

void push_stack(struct Stack *stack, int new) {
  stack->head++;
  stack->data[stack->head - 1] = new;
}

int pop_stack(struct Stack *stack, int *data) {
  if (stack->head == 0) return 0;
  *data = stack->data[--stack->head];
  return 1;
}

void print_stack(struct Stack *stack) {
  int i;
  if (stack->head == 0) {
    printf("< empty >\n");
    return;
  }

  printf("< ");
  for (i=0; i < (stack->head - 1); i++) {
    printf("%d ", stack->data[i]);
  }
  printf("(%d) >\n", STACK_HEAD(stack));
}

#define DEF_OP(op) {\
    int x, y;\
    if (!pop_stack(stack, &x)) return 0;\
    if (!pop_stack(stack, &y)) { push_stack(stack, x); return 0; }\
    push_stack(stack, x op y);\
    return 1;\
  }

int run_stack(struct StackOp *op, struct Stack *stack) {
  switch (op->type) {
    case CONST: { 
      push_stack(stack, op->data);
      return 1;
    }
    case ADD: { DEF_OP(+) }
    case SUB: { DEF_OP(-) }
    case MULT: { DEF_OP(*) }
    case CLR: { 
      stack->head = 0;
    }
    default: {
      return 1;
    }
  }
}

// reverse polish notation calculator
// 2 1 2 + * = 6 <- reverse polish notation (stack-based operators)
// (2 * (1 + 2)) = 6  <- normal mathematical notation (infix operators)

int main(void) {
  struct Stack stack;
  struct StackOp op;
  char *input;
  size_t len;
  stack.head = 0;

  while (true) {
    print_stack(&stack);
    printf("--> ");
    getline(&input, &len, stdin);
    read_op(&op, input);
    if (op.edge) {
      switch (op.type) {
        case ERROR: {
          printf(
            "Parse error! Please input one of {+,*,-,^,<digits>}.");
          break;
        }
        case TERM: { 
          printf("The result is: %d!\n", STACK_HEAD(&stack));
          printf("Exiting.");
          return 1;
        }
        default: {} 
      }
    } else {
      if (!run_stack(&op, &stack)) printf("Operation error!\n");
    }
  }
}

