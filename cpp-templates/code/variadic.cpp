#include <iostream>
#include <type_traits>

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

template <typename... Ts>
struct all_the_same : std::true_type { };

template <typename T, typename... Ts>
struct all_the_same<T, T, Ts...> : all_the_same<T, Ts...> { };

template <typename T, typename U, typename... Ts>
struct all_the_same<T, U, Ts...> : std::false_type { };

int main(int argc, char* argv[])
{
    log("hello", "world");
    log(42, 3.14, true, 'X');

    return 0;
}
