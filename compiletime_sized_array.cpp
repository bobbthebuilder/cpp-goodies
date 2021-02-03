/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <span>
#include <iostream>

template <typename T, std::size_t N>
class compiletime_sized_array
{
public:
    using value_type       = T;
    using span_type        = typename std::span<T, N>;
    using iterator_type    = typename span_type::iterator;
    using difference_type  = typename std::iterator_traits<iterator_type>::difference_type;
    /**
     * Compiletime-sized array with automatic storage duration that requires C++20.
     **/
    constexpr compiletime_sized_array() noexcept
    : data_range(data)
    , last(data_range.begin())
    {
    }
    constexpr difference_type length() const noexcept
    {
        return std::distance(begin(), end());
    }
    constexpr std::size_t capacity() const noexcept
    {
        return data_range.size();
    }
    constexpr iterator_type begin() const noexcept
    {
        return data_range.begin();
    }
    constexpr iterator_type end() const noexcept
    {
        return last;
    }
    void emplace_back(const value_type& value)
    {
        if (last != data_range.end())
        {
            *last = value;
            ++last;
        }
    }
    void emplace_back(value_type&& value)
    {
        if (last != data_range.end())
        {
            *last = std::move(value);
            ++last;
        }
    }
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        (emplace_back(std::forward<Args>(args)), ...);
    }
    void pop_back()
    {
        if (last != data_range.begin())
        {
            --last;
        }
    }
private:
    value_type data[N];
    span_type data_range;
    iterator_type last;
};

int main()
{
    {
        compiletime_sized_array<int, 4> numbers;
        numbers.emplace_back(10, 10, 3, 2, 19, 44); // 10, 10, 3, 2
        numbers.pop_back();                         // 10, 10, 3
        numbers.emplace_back(5);                    // 10, 10, 3, 5
        std::cout << "numbers:\n";
        for (const auto& number : numbers) { std::cout << number << " "; }
        std::cout << "\n";
    }
    {
        compiletime_sized_array<std::pair<const char*, int>, 100> log;
        log.emplace_back(std::make_pair("log msg1", 155));
        log.emplace_back(std::make_pair("log msg2", 156));
        log.emplace_back(std::make_pair("log msg3", 157));
        log.emplace_back(std::make_pair("log msg4", 158));

        std::cout << "\nlogfile contains:\n";
        for (const auto& l : log) { std::cout << l.first << ", " << l.second << "\n"; }
    }
}
