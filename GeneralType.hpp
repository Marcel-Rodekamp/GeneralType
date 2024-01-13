#pragma once

#include<iostream>
#include<iomanip>
#include<type_traits>
#include<variant>
#include<concepts>
#include<complex>
#include<vector>

#include <cstdlib>
#include <memory>
#include <cxxabi.h>


//! A function to convert a Type to a string representation.
// ToDo: This requires a lot hard coding, can we automate this?
template<typename Type> 
std::string typeToString(){
    if constexpr( std::is_same_v<Type,bool>){return "bool";}
    
    if constexpr( std::is_same_v<Type,int>){return "int";}

    if constexpr( std::is_same_v<Type,float>){return "float";}
    if constexpr( std::is_same_v<Type,double>){return "double";}
    
    if constexpr( std::is_same_v<Type,std::complex<float>>){return "complex<float>";}
    if constexpr( std::is_same_v<Type,std::complex<double>>){return "complex<double>";}
    
    if constexpr( std::is_same_v<Type,std::vector<bool>>){return "vector<bool>";}
    if constexpr( std::is_same_v<Type,std::vector<int>>){return "vector<int>";}
    if constexpr( std::is_same_v<Type,std::vector<float>>){return "vector<float>";}
    if constexpr( std::is_same_v<Type,std::vector<double>>){return "vector<double>";}
    if constexpr( std::is_same_v<Type,std::vector<std::complex<float>>>){return "vector<complex<float>>";}
    if constexpr( std::is_same_v<Type,std::vector<std::complex<double>>>){return "vector<complex<double>>";}
    // ...

    // If type has no explicit implementation we just use the c++ type name
    return typeid(Type).name();
}


// This anonymous namespace is used to implement the concepts that check weather a type 
// contains certain operators. These are then required to streamline the operators of the 
// GeneralType to the Type specific operators
namespace {

//! A concept that checks weather a type has a streaming operator `operator<<`
template<typename T> 
concept hasStreamingOperator = requires(std::ostream &os, T t) {
    { os << t } -> std::convertible_to<std::ostream &>;
};

//! A concept that checks weather a type has a negation operator `operator!`
template<typename T> 
concept hasNegationOperator = requires(T t) {
    { !t } -> std::convertible_to<bool>;
};

//! A concept that checks weather a type has a negation operator `operator!`
template<typename T> 
concept hasDereferenceOperator = requires(T t) {
    { *t } -> std::convertible_to<decltype(*t)>;
};

//! A concept that checks weather a type has a prefix increment `operator++`
template<typename T> 
concept hasPrefixIncrementOperator = requires(T t) {
    { ++t } -> std::convertible_to<decltype(++t)>;
};

//! A concept that checks weather a type has a postfix increment `operator++(int)`
template<typename T> 
concept hasPostfixIncrementOperator = requires(T t) {
    { t++ } -> std::convertible_to<decltype(t++)>;
};

//! A concept that checks weather a type has a prefix decrement `operator--`
template<typename T> 
concept hasPrefixDecrementOperator = requires(T t) {
    { --t } -> std::convertible_to<decltype(--t)>;
};

//! A concept that checks weather a type has a postfix decrement `operator--(int)`
template<typename T> 
concept hasPostfixDecrementOperator = requires(T t) {
    { t-- } -> std::convertible_to<decltype(t--)>;
};

//! A concept that checks if two types are addable 
template<typename T, typename U>
concept areAddable = requires(T t, U u) {
    { t + u } -> std::convertible_to<decltype(t + u)>;
};

}

/*! 
 * Implementation of a holder class that can hold any of the types specified in the template parameter pack
 * It is a wrapper around the std::variant class that implements a type cast operator to the desired type as well as some convenience functions for simpler usage
 */
template<typename ... Types_>
class GeneralType{
    public:
    //! Default-construct a `GeneralType`
    GeneralType() = default;

    //! Copy-construct a `GeneralType`
    GeneralType( const GeneralType<Types_...> & genT) = default;

    //! Move-construct a `GeneralType`
    GeneralType( GeneralType<Types_...> && genT) = default;

    //! Copy-assign a `GeneralType`
    GeneralType<Types_...> & operator=( const GeneralType<Types_...> & genT) = default;

    //! Move-assign a `GeneralType`
    GeneralType<Types_...> & operator=( GeneralType<Types_...> && genT) = default;

    //! Copy-Construct the GeneralType<Types...> from an object with type Type;
    template<typename Type>
    GeneralType( const Type & obj ) :
        obj_(obj)
    {}

    //! Move-Construct the GeneralType<Types...> from an object with type Type;
    template<typename Type>
    GeneralType( Type && obj ) :
        obj_(std::move(obj))
    {}

    //! Copy-assign the GeneralType<Types...> from an object with type Type;
    template<typename Type>
    GeneralType<Types_...> & operator=( const Type & obj ){
        obj_ = obj;
        return *this;
    }

    //! Move-assign the GeneralType<Types...> from an object with type Type;
    template<typename Type>
    GeneralType<Types_...> & operator=( Type && obj ){
        obj_ = std::move(obj);
        return *this;
    }

    //! This operator decomposes the `GeneralType` into a given type potentially casting it
    template<typename Type>
    operator Type(){
        return std::visit(
            [](auto & e){
                if constexpr (std::is_convertible_v<Type,decltype(e)>){
                    return static_cast<Type>(e);
                } else if constexpr (std::is_constructible_v<Type,decltype(e)>){
                    return Type(e);
                } else {
                    throw std::runtime_error(
                        "Can not convert held type ("
                        + typeToString<decltype(e)>()
                        +") to desired Type ("+typeToString<Type>()+")"
                    );
                    return Type();
                }
            },
            obj_
        );
    }

    //! An implementation that puts the content of `GeneralType` to the out stream `os`
    friend std::ostream & operator<< (std::ostream & os, GeneralType<Types_...> genT){
        std::visit(
            [&os,&genT](auto & arg){
                if constexpr (hasStreamingOperator<decltype(arg)>){
                    os << arg;
                } else {
                    throw std::runtime_error(
                        "Can not  invoke operator<< held type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                }
            },
            genT.obj_
        );

        return os;
    }

    // The following part of this class implements different operators that can come in handy. The general idea is
    // if the contained type implements the operator then the General Type should call that otherwise evaluate a static_assert

    // =========================================================================================
    // Unary Operators
    // =========================================================================================
    
    //! Negation operator, forwards to the negation operator of the held type
    GeneralType<Types_...> operator!(){
        return std::visit(
            [](const auto & arg){
                if constexpr( hasNegationOperator<decltype(arg)> ){
                    return GeneralType<Types_...>(!arg);
                } else {
                    throw std::runtime_error(
                        "Can not invoke operator! on held type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                    return GeneralType<Types_...>(arg);
                }
            },
            this->obj_
        );
    }

    //! Dereference operator, forwards to the dereference operator of the held type
    GeneralType<Types_...> operator*(){
        return std::visit(
            [](const auto & arg){
                if constexpr( hasDereferenceOperator<decltype(arg)> ){
                    return GeneralType<Types_...>(*arg);
                } else {
                    throw std::runtime_error(
                        "Can not invoke operator! on held type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                    return GeneralType<Types_...>(arg);
                }
            },
            this->obj_
        );
    }

    //! Prefix increment operator, forwards to the prefix increment operator of the held type
    GeneralType<Types_...> operator++(){
        return std::visit(
            [](auto & arg){
                if constexpr( hasPrefixIncrementOperator<decltype(arg)> ){
                    return GeneralType<Types_...>(++arg);
                } else {
                    throw std::runtime_error(
                        "Can not invoke prefix operator++ on held type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                    return GeneralType<Types_...>(arg);
                }
            },
            this->obj_
        );
    }

    //! Postfix increment operator, forwards to the prefix increment operator of the held type
    GeneralType<Types_...> operator++(int){
        return std::visit(
            [](auto & arg){
                if constexpr( hasPostfixIncrementOperator<decltype(arg)> ){
                    return GeneralType<Types_...>(arg++);
                } else {
                    throw std::runtime_error(
                        "Can not invoke postfix operator++ on held type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                    return GeneralType<Types_...>(arg);
                }
            },
            this->obj_
        );
    }

    //! Prefix decrement operator, forwards to the prefix increment operator of the held type
    GeneralType<Types_...> operator--(){
        return std::visit(
            [](auto & arg){
                if constexpr( hasPrefixIncrementOperator<decltype(arg)> ){
                    return GeneralType<Types_...>(--arg);
                } else {
                    throw std::runtime_error(
                        "Can not invoke prefix operator++ on held type (" 
                        + typeToString<decltype(arg)>() + ")"
                    );
                    return GeneralType<Types_...>(arg);
                }
            },
            this->obj_
        );
    }

    //! Postfix decrement operator, forwards to the prefix increment operator of the held type
    GeneralType<Types_...> operator--(int){
        return std::visit(
            [](auto & arg){
                if constexpr( hasPostfixIncrementOperator<decltype(arg)> ){
                    return GeneralType<Types_...>(arg--);
                } else {
                    throw std::runtime_error(
                        "Can not invoke postfix operator++ on held type (" 
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
    
    //! Addition operator, forwards to the addition operator of the held type
    GeneralType<Types_...> operator+(GeneralType<Types_...> rhs){
        return std::visit(
            [&rhs](auto & lhs_arg){
                return std::visit(
                    [&lhs_arg] (auto & rhs_arg){
                        if constexpr ( areAddable<decltype(lhs_arg),decltype(rhs_arg)> ){
                            return GeneralType<Types_...>(lhs_arg + rhs_arg);
                        } else {
                            throw std::runtime_error(
                                "Can not invoke operator+ on held types (" 
                                + typeToString<decltype(lhs_arg)>() + " and " 
                                + typeToString<decltype(rhs_arg)>() + ")"
                            );
                            return GeneralType<Types_...>(lhs_arg);
                        }
                    },
                    rhs.obj_
                );
            },
            this->obj_
        );
    }

    private:
    // Store the held element in a std::variant
    // The std::variant is the heart of this implementation, basically that is what the EntryImpl boils down to
    std::variant<Types_...> obj_;
};
