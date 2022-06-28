# c_rpmcalc
reverse polish notation calculator to teach me some C

The calculator currently supports the following operations:
- addition: 3 + 2
- substraction: 3 - 2
- multiplication: 3 * 2
- division: 3 / 2
- power: pow(3, 2)

### notation
The calculator uses reverse polish notation:

```rpncalc 3 2 *``` this calculates 3 * 2 = 6

```rpncalc 3 2 * 5 +``` this calculates 3 * 2 + 5 = 11

More on reverse polish notation on [Wikipedia](https://en.wikipedia.org/wiki/Reverse_Polish_notation).

**Word of advise:** Due to how the shell works, you might have to escape some operations, i.e. ```rpncalc 3 2 \*``` for multiplication.

### implementation
The calculator is implemented as a stack machine using a double-linked list.

The main goal of the program was to teach me linked lists and input argument handling.

## dependencies
- requires the pcre library
- requires the math library

## compiling
gcc -o rpncalc main.c -lpcre -lm
