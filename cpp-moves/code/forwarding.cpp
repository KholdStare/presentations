#include <utility>
#include <algorithm>
#include <iostream>

void log()
{
    std::cout << std::endl;
}

template <typename T, typename... Ts>
void log(T const& val, Ts const&... rest)
{
    std::cout << val << " ";
    log(rest...);
}

template <typename F, typename... Args>
auto logAndCall1(F& func, Args&... args)
{
    log(args...);
    return func(args...);
}

namespace
{
template <typename F, typename... Args>
auto logAndCall2(F& func, Args&&... args)
{
    log(args...);
    return func(std::forward<Args>(args)...);
}
}

namespace
{
class min_t
{
public:
    constexpr min_t() { }

    template <typename T>
    T operator() (T const& a, T const& b) const
    {
        return std::min(a, b);
    }
};

constexpr min_t min{};

template <typename T>
T anothermin(T const& a, T const& b);
}

int main(int argc, char* argv[])
{
    int a = 3;
    int b = 4;
    logAndCall1(min, a, b);
    // logAndCall1(min, 42, 23);

    logAndCall2(min, 42, 23);
    logAndCall2(min, a, b);

    return 0;
}
