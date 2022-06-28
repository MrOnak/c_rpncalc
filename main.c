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
  int intval;
  float floatval;
  char *operand;
  bool isInt;
  bool isFloat;
  bool isOperand;
  struct node * next;
  struct node * prev;
} node_t;

node_t * head = NULL;
int lastInt; // holds regex match from last successful integer test
float lastFloat; // holds regex match from last successful float test

bool testForInt(char *subject) {
  bool returnValue = false;

  const char *err;
  int errOffset;
  int ovector[30];
  pcre *regex = pcre_compile("^[\\+\\-]*[1-9]+[0-9]*$", 0, &err, &errOffset, NULL);
  int rc = pcre_exec(regex, NULL, subject, strlen(subject), 0, 0, ovector, 30);

  if (rc == PCRE_ERROR_NOMATCH) {
    //printf("not an integer ");
  } else if (rc < -1) {
    printf("Error: %d from int regex ", rc);
    exit(1);
  } else {
    lastInt = atoi(subject);
    returnValue = true;
  }

  return returnValue;
}

bool testForFloat(char *subject) {
  bool returnValue = false;

  const char *err;
  int errOffset;
  int ovector[30];
  pcre *regex = pcre_compile("^[\\+\\-]*[0-9]*\\.[0-9]*$", 0, &err, &errOffset, NULL);
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
    if (testForInt(current->rawval)) {
      current->intval = lastInt;
      current->rawval = NULL;
      current->isInt = true;
      //printf("integer: %d\n", current->intval);

    } else if (testForFloat(current->rawval)) {
      current->floatval = lastFloat;
      current->rawval = NULL;
      current->isFloat = true;
      //printf("float: %f\n", current->floatval);

    } else if (strlen(current->rawval) == 1) {
      switch (*current->rawval) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '%':
          current->operand = current->rawval;
          current->rawval = NULL;
          current->isOperand = true;
          //printf("operand: %s\n", current->operand);
          break;
        default:
          printf("Error: unrecognized argument %s\n", current->rawval);
          exit(1);
      }
    } else {
      printf("Error: unrecognized argument %s\n", current->rawval);
      exit(1);
    }

    current = current->next;
  } while (current->next != NULL);
}

void debugStack() {
  uint8_t i = 0;
  node_t * current = head;

  do {
    printf("%d", i);
    if (current->isInt) {
      printf("  (int)    %d\n", current->intval);
    } else if (current->isFloat) {
      printf("  (float)  %f\n", current->floatval);
    } else if (current->isOperand) {
      printf("  (string) %s\n", current->operand);
    }

    current = current->next;
    i++;
  } while(current->next != NULL);
}

void calculate() {
  uint8_t i = 0;
  node_t * current = head;

  do {
    if (current->isOperand) {
      // right now all operands take two arguments so we do not need to test for variance
      if (i >= 2) {
      } else {
        printf("Error: operand without enough arguments\n");
        exit(1);
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
      current->isInt = false;
      current->isFloat = false;
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
    debugStack();
    calculate();
    debugStack();

  } else {
    printf("Usage example: rpncalc 3 2 +\n this would calculate 3 + 2 = 5\n\n");
  }

  exit(EXIT_SUCCESS);
}
