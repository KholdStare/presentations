struct true_type {
    static constexpr bool value = true;
};

struct false_type {
    static constexpr bool value = false;
};

template <typename T>
struct is_pointer : false_type { };

template <typename T>
struct is_pointer<T*> : true_type { };

static_assert(is_pointer<int*>::value == true, "");
static_assert(is_pointer<char>::value == false, "");
//static_assert(is_pointer<char*>::value == false, "");


template <typename T, typename U>
struct is_same : false_type { };

template <typename T>
struct is_same<T, T> : true_type { };

static_assert(is_same<int, int>::value == true, "");
static_assert(is_same<char, int>::value == false, "");

template <typename T>
struct remove_const
{
    using type = T;
};

template <typename T>
struct remove_const<T const>
{
    using type = T;
};

static_assert(is_same<int, remove_const<int const>::type>::value, "");

int main(int argc, char* argv[])
{
    return 0;
}
