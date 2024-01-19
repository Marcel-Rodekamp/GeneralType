#include "../GeneralType.hpp"
#include<map>

typedef GeneralType<
    bool, int, double, std::string
> GenType;

typedef std::map<std::string,GenType> Dict;

int main(){
    /*!
     * A main motivation of this class is to implement a dictionary that is able to hold arbitrary
     * types as values (and referenced with strings). Similar to what one can find in Python dicts.
     *
     * We can achive this by the above typedef 
     * ```
     * typedef std::map<std::string,GenType> dict;
     * ```
     * which utilizes the std::map for the heavy lifting and stores pairs of std::string and GenType.
     *
     * */

    // suppose you have a parameter file (yml,json,...) and you subsequently read in the parameters
    // as pair of std::string and some type. We can fill a dict like this 
    Dict myParameters;
    
    // let us read in a bool flag
    myParameters["key1"] = true;
    // let us read in a double parameter
    myParameters["key2"] = 1.235;
    // let us, finally, read in a int parameter
    myParameters["key3"] = 2;

    // sometimes you have a parameter that is computed out of two parameters:
    myParameters["key4"] = myParameters["key3"]*myParameters["key2"];

    // alternatively you can compute parameters with non-GenType numbers as long as the underlying
    // types can be computed
    myParameters["key5"] = myParameters["key3"]*0.2;

    // And maybe we want to log what we "read in"
    for(auto [key,value]: myParameters){
        // be careful that the read in types are printable like this or you may need to take 
        // extra care, for example with std::vectors. 
        std::cout << std::boolalpha << key << ": " << value << std::endl;
    }

}
