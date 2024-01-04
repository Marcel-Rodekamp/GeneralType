#include<iostream>
#include<iomanip>
#include<variant>

// This is used as a test type which can do basically nothing than exist
struct NoneImpl{
    friend std::ostream & operator<< (std::ostream & os, NoneImpl none){
        os << "None"; return os; 
    }
};

template<typename Type> 
std::string typeToString(){
    if constexpr( std::is_same_v<Type,int>){return "int";}
    if constexpr( std::is_same_v<Type,int>){return "float";}
    if constexpr( std::is_same_v<Type,int>){return "double";}
    if constexpr( std::is_same_v<Type,NoneImpl>){return "None";}
    // ...

    return "void";
}

template<typename ... Types_>
class GeneralType{
    public:
    //! Construct the GeneralType<Types...> from an object with type Type;
    template<typename Type>
    GeneralType( const Type & obj) :
        obj_(obj)
    {}

    //! Copy-construct a `GeneralType`
    GeneralType( const GeneralType<Types_...> & genT) = default;

    //! Move-construct a `GeneralType`
    GeneralType( GeneralType<Types_...> && genT) = default;

    //! This operator decomposes the `GeneralType` into a given type potentially casting it
    template<typename Type>
    operator Type(){
        // we can use std::visit to decompose the std::variant into its contained object (C++17) -> result
        // If helt type is convertible to Type we static_cast the result to Type
        // If helt type is only constructible from Type we construct the Type from result
        // If none of this works, a runtime error is evaluated.
        return std::visit(
            [](auto & e){
                if constexpr (std::is_convertible_v<Type,decltype(e)>){
                    return static_cast<Type>(e);
                } else if constexpr (std::is_constructible_v<Type,decltype(e)>){
                    return Type(e);
                } else {
                    throw std::runtime_error(
                        "Can not convert helt type ("
                        + typeToString<decltype(e)>()
                        +") to desired Type ("+typeToString<Type>()+")"
                    );
                    return Type();
                }
            },
            obj_
        );
    }

    //! A implementation that puts the content of `GeneralType` to the out stream `os`
    friend std::ostream & operator<< (std::ostream & os, GeneralType<Types_...> genT){
        std::visit(
            [&os,&genT](auto & arg){os << arg;},
            genT.obj_
        );

        return os;
    }

    // The following part of this class implements different operators that can come in handy. The general idea is
    // if the contained type implements the operator then the General Type should call that otherwise evaluate a static_assert

    // =========================================================================================
    // Unary Operators
    // =========================================================================================
    template <typename T>
    struct HasNegationOperator {
        template <typename U>
        static auto test(U u) -> decltype(!u);

        static std::false_type test(...);

        static constexpr bool value = std::is_same<decltype(test(std::declval<T>())), bool>::value;
    };
    GeneralType<Types_...> operator!(){
        return std::visit(
            [](const auto & arg){
                if constexpr( HasNegationOperator<decltype(arg)>::value ){
                    return GeneralType<Types_...>(!arg);
                } else {
                    throw std::runtime_error(
                        "Can not invoke operator! on helt type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                    return GeneralType<Types_...>(arg);
                }
            },
            this->obj_
        );
    }


    // =========================================================================================
    // Binary Operators
    // =========================================================================================

    private:
    // Store the held element in a std::variant
    // The std::variant is the heart of this implementation, basically that is what the EntryImpl boils down to
    std::variant<Types_...> obj_;
};

typedef GeneralType<
    bool,int,float,double, NoneImpl
> GenType;

int main(int argc, char** argv){
    // Construct a general type
    GenType BOOL = true; 
    GenType INT = 1;
    GenType FLOAT = float(3.141592653589793 );
    GenType DOUBLE = double(3.141592653589793 );
    GenType NONE = NoneImpl();


    // We can copy and move construct
    GenType FLOAT_COPY(FLOAT);
    GenType FLOAT_MOVE(std::move(FLOAT_COPY));

    // We can type cast if things are typecastable
    float double_cast = DOUBLE;

    // We can print the values using the streaming operator
    std::cout << std::boolalpha << BOOL << std::endl;
    std::cout << INT << std::endl;
    std::cout << std::setprecision(8) << FLOAT << std::endl;
    std::cout << std::setprecision(16) << DOUBLE << std::endl;

    // we can use all kinds of unary operators, calling the underlying operators, 
    // - Negation
    !BOOL;

    // Should we call a operator which is not implemented by a helt Type a runtime error is thrown
    try {
        !NONE;
    } catch(std::runtime_error e) {
        std::cout << e.what() << std::endl;
    };

}
