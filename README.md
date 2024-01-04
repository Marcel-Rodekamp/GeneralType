# GeneralType

This code introduces a versatile homogeneous container, `GenType`, implemented using `std::variant`. To utilize this implementation, define a typedef `GeneralType<Types...>` listing all acceptable types. The `GenType` implements various functions designed to be type-agnostic, promoting simplicity and readability in usage.

It is advisable to avoid using this in performance-critical code due to the underlying mechanism, `std::variant`, which may not be optimal in such contexts.

## Supported Operators

The code supports a variety of operators and functions, harnessing the operators of the underlying types:

* Unary Operators
    - `operator!`: Negation operator
* Binary Operators (ToDo)
* Typecasting
    - `operator Type()`: Typecasting to an arbitrary type; Security measures are in place to facilitate type casting.
* I/O
    - `operator<<`: Streaming Operator


I aim to extend this implementation.
