#include "GeneralType.hpp"

#include <complex>
#include <vector>

// Define a custom type by specifying the expected types
typedef GeneralType<
    bool,int,
    float,double,
    std::complex<float>,std::complex<double>,
    std::vector<bool>, std::vector<int>,std::vector<float>,std::vector<double>,
    std::vector<std::complex<float>>,std::vector<std::complex<double>>,
    double*
    // This list can be arbitrarily extended
    //
    // limitations are coming from the underlying std::variant. It can not hold
    // references, arrays, or the type void
> GenType;

int main(int argc, char** argv){
    /*!
     * We can construct a GenType object with any kind of value 
     * as long as the type is in the list of allowed types above.
     * Here are a few examples:
     * */
    GenType BOOL = true; 
    GenType INT  = 1; 
    GenType DOUBLE = double(3.141592653589793 );
    GenType VECTOR_D = std::vector<double>{1,2,3,4,5,6,7,8,9,10};
    double local_d;
    GenType DOBLE_PTR = &local_d;

    /*!
     * The usage of the class should be as easy as possible. 
     * So if possible we don't want to cast the `GenType` to it's held type. 
     * Instead we want to use the operators/functions of the held type directly.
     * Thus we implement those operators such that they streamline the call to the 
     * underlying operators of the held type.
     * Nevertheless, all operators try to return a `GenType`. 
     * You should make sure that if a Type specific operator returns a different type, 
     * e.g. `operator! -> bool`, 
     * the requires type must be in the list of allowed types.
     *
     * Here is a list of the implemented operators:
     * */

    //! * Streaming Operator `operator<<`
    std::cout << std::boolalpha << BOOL << std::endl;
    std::cout << DOUBLE << std::endl;

    //! * Negation Operator `operator! -> GenType`
    !BOOL;
    !DOUBLE;

    //! * Dereference Operator `operator* -> GenType`
    *DOBLE_PTR;

    //! * Address Operator `operator& -> GenType*`
    GenType * GENTYPE_PTR = &DOUBLE;

    //! * Prefix Increment Operator `operator++ -> void`
    ++INT;

    //! * Postfix Increment Operator `operator++(int) -> void`
    INT++;

    //! * Prefix Decrement Operator `operator-- -> GenType`
    --INT;

    //! * Postfix Decrement Operator `operator--(int) -> GenType`
    INT--;

    //! * Implicit/Explicit Type conversion `operator T() -> T`
    double d = INT;

    //! * Addition Operator `operator+ -> GenType`
    INT + DOUBLE;

    //! * Subtraction Operator `operator- -> GenType`
    INT - DOUBLE;

    //! * Multiplication Operator `operator* -> GenType`
    DOUBLE * DOUBLE;

    //! * Division Operator `operator/ -> GenType`
    DOUBLE / DOUBLE;

    //! * Modulus Operator `operator% -> GenType`
    INT % INT;

    //! * Bitwise AND Operator `operator& -> GenType`
    BOOL & BOOL;

    //! * Logical AND Operator `operator&& -> GenType`
    BOOL && BOOL;

    //! * Exclusive OR Operator `operator^ -> GenType`
    BOOL ^ BOOL;

    //! * Bitwise inclusive Or  OR Operator `operator| -> GenType`
    BOOL | BOOL;

    //! * Logical inclusive Or  OR Operator `operator|| -> GenType`
    BOOL || BOOL;

    //! * Smaller Compare Operator `operator< -> GenType`
    DOUBLE < DOUBLE;

    //! * Larger Compare Operator `operator> -> GenType`
    DOUBLE > DOUBLE;

    //! * Smaller Equal Compare Operator `operator<= -> GenType`
    DOUBLE <= DOUBLE;

    //! * Larger Equal Compare Operator `operator>= -> GenType`
    DOUBLE >= DOUBLE;

    //! * Equality Operator `operator== -> GenType`
    DOUBLE == DOUBLE;

    //! * Inequality Operator `operator=! -> GenType`
    DOUBLE != DOUBLE;
    
    /*! In case a held type doesn't implement an operator, 
     * like the `std::vector` has no nativ `operator<<`, 
     * a runtime (!) error is thrown.
     * */
    /*
    try{
        std::cout << VECTOR_D << std::endl;
    } catch(std::runtime_error e){
        std::cout << e.what() << std::endl;
    }
    */
}
