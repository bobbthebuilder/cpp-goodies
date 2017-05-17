/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

std::vector<std::string> names;

/// no argument copy needed + s is lvalue?
/// -> pass by const lvalue reference is optimal
// s is lvalue: no-cost
// s is rvalue: 1x copy
void f1 (const std::string& s)
{
    names.push_back(s);
}

/// argument copy needed?
/// -> consider pass by value
// s is lvalue: 1x copy, 1x move
// s is rvalue: 2x move
void f2 (std::string s)
{
    names.push_back(std::move(s));
}

/// unclear if argument copy needed?
/// -> overload for lvalues and rvalues
// s is lvalue: 1x copy
// s is rvalue: 1x move
// disadvantage: does not scale for multiple arguments due to 2^N overloads for N arguments,
//               two functions in the object code.
void f3 (const std::string& s)
{
    names.push_back(s);
}
void f3 (std::string&& s)
{
    names.push_back(std::move(s));
}

/// unclear if argument copy needed?
/// -> forward reference (called universal reference)
// s is lvalue: 1x copy
// s is rvalue: 1x move
// disadvantages: it may yield several functions in the object code,
//                there are argument types that can't be passed by universal reference
template <class T>
void f4 (T&& s)
{
    names.push_back(std::forward<T>(s));
}

//// Rule of thumb ////
/*
 *
 *                                               +-- yes --> pass by value
 *                                               |
 * +---------------------------------------------+
 * |does the function conceptionally make a copy?|
 * +---------------------------------------------+
 *                                               |
 *                                               +-- no  --> pass by const reference or lvalue/rvalue overloads
 *
 */

int main()
{
    std::string s = "foo";

    f1(s);                  // lvalue
    f1(std::string("foo")); // rvalue
    f1("foo");              // rvalue

    f2(s);
    f2(std::string("foo"));
    f2("foo");

    f3(s);
    f3(std::string("foo"));
    f3("foo");

    f4(s);
    f4(std::string("foo"));
    f4("foo");
}
