/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <string>
#include <iostream>
#include <type_traits>
#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

// reference collapsing rules:
// T& &   -> T&
// T& &&  -> T&
// T&& &  -> T&
// T&& && -> T&&

/* schematic implementation of std::move:
template <class T> decltype(auto) move(T&& param)
{
    using ReturnType = std::remove_reference_t<T>&&;
    return static_cast<ReturnType>(param);
}
// -> if T is lvalue: return T&&
// -> if T is rvalue: return T&&
*/

/* schematic implementation of std::forward:
template <class T> T&& std::forward(T&& param)
{
    return static_cast<T&&>(param);
}
// -> if T is lvalue: return T&
// -> if T is rvalue: return T&&
*/

// x is a forwarding reference in f
//
template<class T>
void f(T&& x)
{
    std::cout << "x deduced to: " << type_id_with_cvr<decltype(x)>().pretty_name() << " | ";
    std::cout << "std::forward<T>(x) deduced to: " << type_id_with_cvr<decltype(std::forward<T>(x))>().pretty_name() << '\n';
}

struct A
{
    A(std::string&& s)
    : s_(std::move(s))
    {
        std::cout << "move ctor\n";
    } // simple move constructor

    A& operator=(A&& other)
    {
        std::cout << "move assignment operator\n";
        s_ = std::move(other.s_);
        return *this;
    } // simple move assignment operator

private:
    std::string s_;
};

class K
{
private:
    std::string str;
public:
    /// version 1:
    //K(const std::string& s) : str(s) { std::cout << "ctor (copying const string) " << str << "\n"; }
    //K(std::string&& s) : str(std::move(s)) { std::cout << "ctor (moving string) " << str << "\n"; }

    /// version 2:
    // enable_if is necessary because for a nonconstant lvalue a perfect forwarding constructor is a better match than the copy constructor.
    template <typename T, typename = std::enable_if_t<std::is_convertible_v<T, std::string>>>
    K(T&& s) : str(std::forward<T>(s)) { std::cout << "template ctor string " << str << "\n"; }

    K(const K& k) : str(k.str) { std::cout << "copy ctor " << str << "\n"; }
    K(K&& k) : str(std::move(k.str)) { std::cout << "move ctor " << str << "\n"; }
};

int main()
{
    int i = 5;
    const int j = 10;
    const int& ref_to_j = j;
    int&& x = 1;        // x is initialized with a prvalue, hence it becomes an *lvalue*

    f(i);               // argument is lvalue.                    call: f<int&>(int&)
    f(j);               // argument is const lvalue.              call: f<const int&>(const int&)
    f(ref_to_j);        // argument is const lvalue reference.    call: f<const int&>(const int&)
    f(10);              // argument is rvalue.                    call: f<int>(int&&)
    f(std::move(x));    // argument is rvalue.                    call: f<int>(int&&)

    A a("testtesttest"), b("");
    b = std::move(a);

    std::string s = "test";
    K k1(s);
    K k2("abcd");
    K k3(k1);
    K k4(std::move(k2));
}
