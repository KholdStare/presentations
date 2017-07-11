#include <cstddef>

template <typename T>
class FixedVector {
    T* data_;
    size_t size_;
public:
    FixedVector(size_t size)
        : data_(new T[size])
        , size_(size)
    { }

    ~FixedVector() {
        delete[] data_;
    }

    T&       operator[] (size_t index)       { return data_[index]; }
    T const& operator[] (size_t index) const { return data_[index]; }

    size_t size() const { return size_; }
};

int main(int argc, char* argv[])
{
    FixedVector<int> intVec{10};
    FixedVector<double> doubleVec{4};

    return 0;
}
