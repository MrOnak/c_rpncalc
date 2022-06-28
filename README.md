# c_rpmcalc
reverse polish notation calculator to teach me some C

The calculator currently supports the following operations:
- addition: 3 2 +
- substraction: 3 2 -
- multiplication: 3 2 *
- division: 3 2 /
- power: 3 2 ^ **or** 3 2 pow
- root: 27 3 root
- modulo: 3 2 % (attention: _modulo operation is done on the integer values of both operands, since the notion of a "remainder" is only applicable to integer division_)

### notation
The calculator uses reverse polish notation:

```rpncalc 3 2 *``` this calculates 3 * 2 = 6

```rpncalc 3 2 * 5 +``` this calculates 3 * 2 + 5 = 11

More on reverse polish notation on [Wikipedia](https://en.wikipedia.org/wiki/Reverse_Polish_notation).

**Word of advise:** Due to how the shell works, you might have to escape some operations, i.e. ```rpncalc 3 2 \*``` for multiplication.

### note on the calculation of powers
There are two ways to calculate a power:

```rpncalc 3 2 ^``` and ```rpncalc 3 2 pow``` both mean the same.

### note on the calculation of root numbers
The trivial way to calculate a square root is ```rpncalc 9 2 root``` which equals ```sqrt(9)```.
You can calculate the fourth root number in the same way: ```rpncalc 256 4 root```.

Due to how root numbers work you don't actually need the ```root``` operator, you can deconstruct it with ```pow```:

```rpncalc 27 3 root``` is mathematically identical to ```rpncalc 27 3 1 / ^```

### implementation
The calculator is implemented as a stack machine using a double-linked list.

The main goal of the program was to teach me linked lists and input argument handling. While it seems to become a somewhat capable calculator, this is, and never will be, the goal. The goal is education.

## dependencies
- requires the pcre library
- requires the math library

## compiling
gcc -o rpncalc main.c -lpcre -lm
