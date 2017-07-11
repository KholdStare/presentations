#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

template <typename X, typename Y>
struct Pair
{
    X first;
    Y second;

};

template <typename X, typename Y>
bool operator < (X const& x, Pair<X, Y> const& pair)
{
    return x < pair.first;
}

template <typename X, typename Y>
bool operator < (Pair<X, Y> const& pair, X const& x)
{
    return pair.first < x;
}

template <typename X, typename Y>
Pair<X, Y> MakePair(X x, Y y)
{
    return Pair<X, Y>{std::move(x), std::move(y)};
}

template <typename Key, typename Value>
class CrappyMap
{
    std::vector<Pair<Key, Value>> items_;

public:
    void insert(Key k, Value v)
    {
        auto it = std::upper_bound(std::begin(items_), std::end(items_), k);
        items_.insert(it, MakePair(std::move(k), std::move(v)));
    }

    Value const* get(Key const& k) const
    {
        auto it = std::lower_bound(std::begin(items_), std::end(items_), k);
        if (it != std::end(items_) && it->first == k) {
            return &it->second;
        }
        return nullptr;
    }
};

int main(int argc, char* argv[])
{
    CrappyMap<std::string, int> names;

    names.insert("hello", 17);
    names.insert("wololo", 42);
    names.insert("trololo", 23);

    assert(names.get("bogus") == nullptr);
    assert(*names.get("hello") == 17);
    assert(*names.get("wololo") == 42);
    assert(*names.get("trololo") == 23);

    std::cout << "hello: " << *names.get("hello") << std::endl;

    return 0;
}
