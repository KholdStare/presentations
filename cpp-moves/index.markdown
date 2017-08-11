---
title: 'C++ Move Semantics'
---

# C++ Move Semantics

## Overview

* Why needed?
* How?
* Perfect forwarding

## Vector

```{.cpp}
class Vector {
    double* storage_;
    size_t  size_;

public:
    Vector(size_t numElements)
        : storage_(new double[numElements])
        , size_(numElements)
    { }

    ~Vector() { delete[] storage_; }

    double& operator[] (size_t i)       { return storage_[i]; }
    size_t size() const { return size_; }
};
```

## {data-background="images/copying-vector.svg"}

## Copy constructor

```{.cpp}
Vector(size_t numElements)
    : storage_(new double[numElements])
    , size_(numElements)
{ }

Vector(Vector const& other)
    : Vector(other.size())
{
    std::copy_n(other.storage_, other.size_, storage_);
}
```

## Operator +

```{.cpp}
??? operator + (Vector const& a, Vector const& b);
```

## Operator +

```{.cpp}
Vector operator + (Vector const& a, Vector const& b)
{
    // create result of same size
    assert(a.size() == b.size());
    Vector result(a.size());

    // compute addition
    std::transform(
        a.begin(), a.end(), // input 1
        b.begin(),          // input 2
        result.begin(),     // result
        std::plus<double>() // binary operation
    );

    return result; // ... but can we avoid the copy?
}
```

## If copy is made

* Make all the effort of copying
* Destroy immediately after

## RVO to the rescue

* Return Value Optimization!
* Copy constructor is elided
* guaranteed by the C++ standard.

## Why do we need to move?

One reason is resources!

* `std::unique_ptr<T>`
* `std::thread` 
* Other resource handles

No way to copy, but need to __transfer ownership__

## Transferring ownership

* Only one user owns data
* Previous owner no longer owns anything

## Move constructor

* A shallow copy
* Nullify the source

## {data-background="images/moving-vector.svg"}

## Move constructor

```{.cpp}
Vector::Vector(Vector&& other)
    // shallow copy
    : storage_(other.storage_)
    , size_(other.size_)
{
    // nullify source
    other.storage_ = nullptr;
    other.size_ = 0;
}
```

## What is &&?

* _Rvalue_ reference
* Special reference that binds to temporary values
* What is an _rvalue_?

## Lvalues and Rvalues

```{.cpp}
Vector a, b, c;
c = a + b;
```

* `a`, `b` and `c` are all *lvalue*s
   * they have a name
* the result of `a + b` is an *rvalue*
   * it is temporary, with no name

## References

```{.cpp}
Vector a, b, c;
Vector&  lref1 = a;     // ok
Vector&  lref2 = a + b; // ERROR
Vector&& rref1 = a + b; // ok
Vector&& rref2 = c;     // ERROR
Vector const& cref = a + b; // ok
```

* `&` is an *lvalue* reference
* `&&` is an *rvalue* reference
* `const &` is special and can also bind and extend the lifetime of temporaries
