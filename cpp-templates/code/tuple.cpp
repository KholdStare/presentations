#include <utility>
#include <iostream>

template <size_t Value>
struct index_constant
{
    static constexpr size_t value = Value;
};

template <typename... Ts>
struct Tuple { };

template <typename T, typename... Ts>
struct Tuple<T, Ts...> : private Tuple<Ts...>
{
private:
    T val_;
public:
    Tuple(T val, Ts... rest)
        : Tuple<Ts...>(std::move(rest)...)
        , val_(std::move(val))
    { }

    template <size_t Index>
    auto& get(index_constant<Index>)
    {
        return Tuple<Ts...>::get(index_constant<Index-1>{});
    }

    T& get(index_constant<0>)
    {
        return val_;
    }
};

template <int Index, typename... Ts>
auto& get(Tuple<Ts...>& t)
{
    return t.get(index_constant<Index>{});
}

namespace
{
    template <typename TupleType>
    struct tuple_size;

    template <typename... Ts>
    struct tuple_size<Tuple<Ts...>>
    {
        static constexpr size_t value = sizeof...(Ts);
    };

    template <typename TupleType>
    struct tuple_size<TupleType&>
        : tuple_size<TupleType>
    { };

    static_assert(tuple_size<Tuple<int, char, double>>::value == 3, "");

    template <size_t... Indices>
    struct index_sequence { };

    template <size_t Index, typename Sequence>
    struct append_to_sequence;

    template <size_t Index, size_t... Indices>
    struct append_to_sequence<Index, index_sequence<Indices...>>
    {
        using type = index_sequence<Indices..., Index>;
    };

    static_assert(std::is_same<
                    index_sequence<1, 2, 3>,
                    append_to_sequence<3, index_sequence<1, 2>>::type
                  >::value, "");

    template <size_t Size>
    struct make_index_sequence;

    template <>
    struct make_index_sequence<0>
    {
        using type = index_sequence<>;
    };

    template <size_t Size>
    struct make_index_sequence
    {
        using type = typename append_to_sequence<
            Size - 1,
            typename make_index_sequence<Size - 1>::type
        >::type;
    };

    static_assert(std::is_same<
                    index_sequence<0, 1, 2, 3>,
                    make_index_sequence<4>::type
                  >::value, "");
}

namespace logger
{
    constexpr struct log_t
    {
        constexpr log_t() { }

        void operator() () const
        {
            std::cout << std::endl;
        }

        template <typename T, typename... Ts>
        void operator() (T const& val, Ts const&... rest) const
        {
            std::cout << val << " ";
            operator()(rest...);
        }
    } log;
}

namespace
{
    template <typename F, typename Tuple, size_t... Indices>
    auto tuple_apply_impl(F&& f, Tuple&& tuple, index_sequence<Indices...>)
    {
        return f(get<Indices>(tuple)...);
    }

    template <typename F, typename Tuple>
    auto tuple_apply(F&& f, Tuple&& tuple)
    {
        constexpr size_t size = tuple_size<Tuple>::value;
        return tuple_apply_impl(f, tuple, typename make_index_sequence<size>::type{});
    }
}

int main(int argc, char* argv[])
{
    Tuple<int, char, double> t{4, 'X', 3.14};

    std::cout << get<1>(t) << std::endl;

    tuple_apply(logger::log, t);

    return 0;
}
