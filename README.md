# GeneralType

This code introduces a versatile homogeneous container, `GenType`, implemented using `std::variant`. To utilize this implementation, define a typedef `GeneralType<Types...>` listing all acceptable types. The `GenType` implements various functions designed to be type-agnostic, promoting simplicity and readability in usage.

It is advisable to avoid using this in performance-critical code due to the underlying mechanism, `std::variant`, which may not be optimal in such contexts.

## Supported Operators

The code supports a variety of operators and functions, harnessing the operators of the underlying types:

* Unary Operators
    - `operator Type`: Typecasting to an arbitrary type; Security measures are in place to facilitate type casting.
    - `operator!`: Negation operator
    - `operator*`: Dereference operator
    - `operator++`: Pre-/Postfix increment operator
    - `operator--`: Pre-/Postfix decrement operator
* Binary Operators 
    - `operator<<`: Streaming Operator
    - `operator+`: Addition operator
    - `operator-`: Subtraction operator
    - `operator*`: Multiplication operator
    - `operator/`: Division operator
    - `operator%`: Modulus operator
    - `operator&`: Bitwise AND operator
    - `operator&&`: Logical AND operator
    - `operator^`: Exclusive OR operator
    - `operator|`: Bitwise inclusive OR operator
    - `operator||`: Logical inclusive OR operator
    - `operator<`: Comparison smaller operator
    - `operator>`: Comparison larger operator
    - `operator<=`: Comparison smaller equal operator
    - `operator>=`: Comparison larger equal operator
    - `operator==`: Comparison equality operator
    - `operator!=`: Comparison inequality operator

## Building

You can build the current version of the code by
```sh
g++ main.cpp -std=c++20 -o main
```
I tested the build with `g++ (GCC) 12.3.0` on a linux machine.
Currently, clang can not be supported as some features of C++20 aren't supported by clang in a way they are used here.
