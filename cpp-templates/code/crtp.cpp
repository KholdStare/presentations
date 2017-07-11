#include <cstdint>
#include <cassert>

template <typename Derived>
struct ComparisonBase
{
private:
    Derived const& getDerived() const {
        return static_cast<Derived const&>(*this);
    }

public:
    bool operator <= (Derived const& other) const {
        return (getDerived() < other) || (getDerived() == other);
    }

    bool operator >= (Derived const& other) const {
        return !(getDerived() < other);
    }

    bool operator != (Derived const& other) const {
        return !(getDerived() == other);
    }

    bool operator > (Derived const& other) const {
        return (getDerived() >= other) && (getDerived() != other);
    }
};

struct OrderEntry : ComparisonBase<OrderEntry>
{
    uint32_t price;
    uint32_t timestamp;

    OrderEntry(uint32_t p, uint32_t t)
        : price(p)
        , timestamp(t)
    { }

    bool operator < (OrderEntry const& other) const {
        return price < other.price
            || ((price == other.price) && timestamp < other.timestamp);
    }

    bool operator == (OrderEntry const& other) const {
        return price == other.price && timestamp == other.timestamp;
    }
};

int main(int argc, char* argv[])
{
    OrderEntry a{1000, 0};
    OrderEntry b{1001, 0};
    OrderEntry c{1001, 10};

    assert(a < b);
    assert(b < c);
    assert(a < c);
    assert(b >= a);
    assert(c != a);

    return 0;
}
