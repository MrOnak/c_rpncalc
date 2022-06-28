#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <stdint.h>
#include <pcre.h>
#include <string.h>
#include <math.h>

typedef struct node {
  char *rawval;
  float floatval;
  char operand;
  bool isNum;
  bool isOperand;
  struct node * next;
  struct node * prev;
} node_t;

node_t * head = NULL;
float lastFloat; // holds regex match from last successful float test

bool testForFloat(char *subject) {
  bool returnValue = false;

  const char *err;
  int errOffset;
  int ovector[30];
  pcre *regex = pcre_compile("^[\\+\\-]?([0-9]*[\\.])?[0-9]+$", 0, &err, &errOffset, NULL);
  int rc = pcre_exec(regex, NULL, subject, strlen(subject), 0, 0, ovector, 30);

  if (rc == PCRE_ERROR_NOMATCH) {
    //printf("not a float ");
  } else if (rc < -1) {
    printf("Error: %d from float regex ", rc);
    exit(1);
  } else {
    lastFloat = atof(subject);
    returnValue = true;
  }

  return returnValue;
}

void parseArguments() {
  node_t * current = head;

  do {
    if (testForFloat(current->rawval)) {
      current->floatval = lastFloat;
      current->isNum = true;
      current->rawval = NULL;
      //printf("float: %f\n", current->floatval);

    } else if (strlen(current->rawval) == 1) {
      switch (*current->rawval) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '%':
          current->operand = *current->rawval;
          current->isOperand = true;
          current->rawval = NULL;
          //printf("operand: %s\n", current->operand);
          break;
        default:
          printf("Error: unrecognized argument %s\n", current->rawval);
          exit(1);
      }
    } else {
      // textual operators
      if (strcmp(current->rawval, "pow") == 0) {
        current->operand = '^';
        current->isOperand = true;
        current->rawval = NULL;
      } else if (strcmp(current->rawval, "root") == 0) {
        current->operand = 'r';
        current->isOperand = true;
        current->rawval = NULL;
      } else {
        printf("Error: unrecognized argument %s\n", current->rawval);
        exit(1);
      }
    }

    current = current->next;
  } while (current->next != NULL);
}

void debugStack() {
  uint8_t i = 0;
  node_t * current = head;

  do {
    printf("%d", i);
    if (current->isNum) {
      printf("  (float) %f\n", current->floatval);
    } else if (current->isOperand) {
      printf("  (oper)  %s\n", &current->operand);
    }

    current = current->next;
    i++;
  } while(current->next != NULL);
  printf("-----------\n");
}

void calculate() {
  uint8_t i = 0;
  node_t * current = head;

  do {
    if (current->isOperand) {
      // right now all operands take two arguments so we do not need to test for variance
      double a2 = current->prev->floatval;
      double a1 = current->prev->prev->floatval;

      switch (current->operand) {
        case '+': current->floatval = a1 + a2; break;
        case '-': current->floatval = a1 - a2; break;
        case '*': current->floatval = a1 * a2; break;
        case '/': current->floatval = a1 / a2; break;
        case '^': current->floatval = pow(a1, a2); break;
        case '%': current->floatval = (int) a1 % (int) a2; break;
        case 'r': current->floatval = pow(a1, 1.0 / a2); break; // any root function
        default:
          printf("Error: unknown operator\n");
          exit(1);
      }
      printf("  %f %s %f == %f\n", a1, &current->operand, a2, current->floatval);

      current->isNum = true;
      current->isOperand = false;
      current->operand = 0;

      if (current->prev->prev->prev == NULL) {
        free(current->prev->prev);
        free(current->prev);
        current->prev = NULL;
        head = current;
      } else {
        current->prev = current->prev->prev->prev;
        current->prev->next = current;
      }
    }


    current = current->next;
    i++;
  } while(current->next != NULL);
}

int main(int argc, char *argv[]) {
  head = (node_t *) malloc(sizeof(node_t));

  if (head == NULL) {
    exit(1);
  }

  if (argc > 1) {
    node_t * current = head;

    // parse all arguments into the stack
    for (uint8_t i = 1; i < argc; i++) {
      current->rawval = argv[i];
      current->isNum = false;
      current->isOperand = false;

      // add new item to stack if there are more arguments
      if (current->next == NULL && i < argc) {
        current->next = (node_t *) malloc(sizeof(node_t));
        current->next->next = NULL;
        current->next->prev = current;

        current = current->next;
      }
    }

    parseArguments();
    //debugStack();
    calculate();

    printf("final result is: %f\n", head->floatval);

  } else {
    printf("Usage example: rpncalc 3 2 +\n this would calculate 3 + 2 = 5\n\n");
  }

  exit(EXIT_SUCCESS);
}
