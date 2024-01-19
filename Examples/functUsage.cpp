#include "../GeneralType.hpp"
#include<concepts>

typedef GeneralType<
    bool, int, double, std::string
> GenType;

void simple(double x){std::cout << x << std::endl;};

void simple_noConvert(auto x){std::cout << x << " " << typeid(x).name() <<  std::endl;}

int main(){
    /*!
     * This example demonstrates how a GenType can be used with different function types.
     * The relevant functions are all defined above, but most of them are just printing the 
     * provided content.
     * 
     * */

    //! First we have a simple function that takes a double; The GenType is simply converted if possible
    simple(GenType(3.14));

    // a std::string can not be converted to double
    try{
        simple(GenType("3.14"));
    } catch(std::runtime_error e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }

    //! If we use a template function with arbitrary templates the GenType itself is just pushed
    simple_noConvert(GenType(3.14));

    //! Currently, there is no automatic way to decompose the GenType in it's original type.
}
