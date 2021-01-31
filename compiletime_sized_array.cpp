#ifndef COMPILETIME_SIZED_ARRAY
#define COMPILETIME_SIZED_ARRAY

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
    friend std::ostream& operator<<(std::ostream& os, const compiletime_sized_array& array)
    {
        for (const auto& i : array)
        {
            os << i << " ";
        }
        return os;
    }
private:
    value_type data[N];
    span_type data_range;
    iterator_type last;
};

int main()
{
    compiletime_sized_array<int, 5> a;
    a.emplace_back(10, 10, 3, 2, 19, 44);
    std::cout << a << "\n";
}

#endif /* COMPILETIME_SIZED_ARRAY */
