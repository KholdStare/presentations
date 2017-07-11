---
title: 'C++ templates'
---

# C++ templates Part 1

## "Kosher" Templates

* Today we talk about the originally intended use of templates
* No meta-programming (yet)

## Generic Programming

* What is lacking in Object Oriented programming?
* Let's look at polymorphism

## Polymorphism

> The provision of a single interface to entities of different types.

* Why do we need it?
* Orthogonality/Modularity
* Extensibility

## Polymorphism

* Subtype polymorphism
* Ad-hoc polymorphism
* Parameteric Polymorphism

# Subtype polymorphism

## Subtype polymorphism

```{.cpp}
class DataOutput {
public:
   virtual ~DataOutput() = 0;
   virtual write(const char* data, size_t size) = 0;
};

class FileOutput : DataOutput {
// ...
};

class NetworkOutput : DataOutput {
// ...
};
```

## {data-background="../images/InheritanceTree.svg"}

## {data-background="../images/InheritanceVenn.svg"}

## Liskov Substitution Principle

https://en.wikipedia.org/wiki/Liskov_substitution_principle

> if S is a subtype of T, then objects of type T in a program may be replaced
> with objects of type S without altering any of the desirable properties of
> that program

Keeps sanity in Object Oriented programming

## Subtype Polymorphism

Inheritance = Subtype polymorphism = OO

# Ad-hoc polymorphism

## Ad-hoc polymorphism

* Same operation for an "ad-hoc" subset of types
* E.g. Absolute value of a number

Problematic in C:

```{.c}
int abs(int);
long labs(long);
long long llabs(long long);
float fabsf(float);
double fabs(double);
long double fabsl(long double);
```

## Ad-hoc polymorphism

Fixed with overloading in C++/Java:

```{.cpp}
int abs(int);
long abs(long);
long long abs(long long);
float abs(float);
double abs(double);
long double abs(long double);
```

## Ad-hoc polymorphism

* Same operation for an "ad-hoc" subset of types
* Can introduce functionality *after the fact*
* No need for base class

# Parametric polymorphism

## Parametric polymorphism

* Same interface for ALL types
* Implies we cannot know anything about the types
* "Bread and butter" generic programming
* First implemented in `CLU` by Barbara Liskov in 1974

## Parametric polymorphism

Containers!

* `std::vector<T>`
* `std::list<T>`
* `std::map<Key, Value>`

## Fixed Int Vector

```{.cpp}
class FixedIntVector {
   int* data_;
   size_t size_;
public:
   FixedIntVector(size_t size)
      : data_(new int[size])
      , size_(size)
   { }

   ~FixedIntVector() {
      delete[] data_;
   }
};
```

## Fixed Int Vector

```{.cpp}
int&       operator[] (size_t index)       { return data_[index]; }
int const& operator[] (size_t index) const { return data_[index]; }

size_t size() const { return size_; }
```

What about storing other types?

## Fixed Vector

```{.cpp}
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
};
```

## Fixed Vector

```{.cpp}
T&       operator[] (size_t index)       { return data_[index]; }
T const& operator[] (size_t index) const { return data_[index]; }

size_t size() const { return size_; }
```

## Template instantiation

> When a template is used, the compiler generates a new concrete type
> for the particular type argument.

## Polymorphism

Summary:

* Subtype polymorphism - inheritance
* Ad-hoc polymorphism - overloading
* Parameteric Polymorphism - templates

# Containers

## Java Case Study

The Collections "Framework"

```{.java}
public class Vector<E>
extends AbstractList<E>
implements List<E>, RandomAccess, Cloneable, Serializable
```

## {data-background="../images/Map_Classes.jpeg"}

## Java Case Study

* Combines parametric polymorphism and subtype polymorphism
* Parametric - polymorphic over elements stored
* Subtype - "taxonomy" of container functionality 

## {data-background="../images/Platypus.jpeg"}

## Java Case Study

* A lot of container functionality is orthogonal
   * e.g. sorting vs finding an element
* Forcing into a hierarchy overly restrictive
* Some methods cannot be properly implemented
   * see "optional methods" in base classes
   * violates Liskov Substitution Principle
* What if we need new functionality?
   
Can we do better?

## [STL Containers](http://en.cppreference.com/w/cpp/container) 

* `array`, `vector`, `deque`, `list`, `forward_list`, `map`, `unordered_map`
* Zero inheritance!
* How?

## STL

* Containers - concrete types with minimal/basic operations
* Algorithms - orthogonal set of operations
* Iterators - the "bridge" between containers and algorithms

## STL

* Add new container? No problem! Algorithms still work
* Add new algorithm? No problem! Containers compatible

## [STL Iterators](http://en.cppreference.com/w/cpp/iterator)

Taxonomy of iterators based on traversal properties:

* InputIterator
* ForwardIterator
* BidirectionalIterator
* RandomAccessIterator
* RandomAccessIterator

## STL

* Keeps each individual container independent/simple
* Distills algorithm requirements into iterators
* How do algorithms use iterators without inheritance?

# Ad-hoc polymorphism revisited

## Templates

Remember the `abs` example?

```{.cpp}
template <typename T>
T abs(T v) {
    return v < T{0} ? -v : v;
}
```

> The subset of supported types is now defined by all types that support
> operations used in the function body.

## Compile-time duck typing

```{.cpp}
template <typename T>
T abs(T v) {
    return v < T{0} ? -v : v;
}
```

Does it have:

* Constructor from `int`
* Comparison `operator <`
* Unary `operator -`

## Quicksort

```{.cpp}
template <typename It>
void quicksort(It begin, It end)
{
    auto numElements = std::distance(begin, end);
    if (numElements < 2) { return; }

    It pivot = partition(begin, end);
    quicksort(begin, pivot);
    quicksort(++pivot, end);
}
```

## Partition

```{.cpp}
template <typename It>
It partition(It begin, It end) {
    auto pivot = *begin;
    It pivotDestination = begin;
    It cursor = begin;

    while (++cursor != end) {
        if (*cursor <= pivot) {
            std::swap(*(++pivotDestination), *cursor);
        }
    }
    std::swap(*pivotDestination, *begin);
    return pivotDestination;
}
```

## Summary/Questions

# More Examples

## Pair

```{.cpp}
template <typename X, typename Y>
struct Pair {
    X first;
    Y second;
};
```

## Crappy Map

```{.cpp}
template <typename Key, typename Value>
class CrappyMap {
    std::vector<Pair<Key, Value>> items_;
public:
    void insert(Key k, Value v);
    Value const* get(Key const& k) const;
};
```

# C++ Templates Part 2 - Metaprogramming

## wololo

trololo

