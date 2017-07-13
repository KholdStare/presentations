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

> When a template is used, the compiler generates a new concrete type/function
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

## Metaprogramming

* What is it?
* Why do it?
* How to do it?

## What is it?

> Code not executed at runtime but compile-time, to aid the programmer

## Why do it?

* To reduce boilerplate
* To enforce constraints
* To precompute things

## How to do it?

* Metafunctions
* Traits
* Tag dispatch
* Variadic templates
* CRTP
* SFINAE

## Low level building blocks

* type aliases
* `constexpr`
* template type substitution
* template specialization
* type inference
* overloading

# Metafunctions

## Metafunctions

> A metafunction is conceptually a function at the type level

## Values at type level

Lift regular values into the type level

```{.cpp}
struct true_type {
    static constexpr bool value = true;
};

struct false_type {
    static constexpr bool value = false;
};
```

Usage:

```{.cpp}
bool b = true_type::value;
```

## Is pointer

Specialize on the case we are trying to detect

```{.cpp}
template <typename T>
struct is_pointer : false_type { };

template <typename T>
struct is_pointer<T*> : true_type { };

static_assert(is_pointer<int*>::value == true, "");
static_assert(is_pointer<char>::value == false, "");
```

## Is same

Specialize on the case we are trying to detect

```{.cpp}
template <typename T, typename U>
struct is_same : false_type { };

template <typename T>
struct is_same<T, T> : true_type { };

static_assert(is_same<int, int>::value == true, "");
static_assert(is_same<char, int>::value == false, "");
```

## Remove const

```{.cpp}
template <typename T>
struct remove_const {
    using type = T;
};

template <typename T>
struct remove_const<T const> {
    using type = T;
};

static_assert(is_same<int, remove_const<int const>::type>::value, "");
```

# Traits

## Traits

How does STL handle iterator "taxonomy"?

* InputIterator
* ForwardIterator
* BidirectionalIterator
* RandomAccessIterator

Let's look at `advance`

## `Advance`

```{.cpp}
template <class InputIt, class Distance>
void advance(InputIt& it, Distance n);
```

* Has to work for vector as well as linked list
* Has to work efficiently

## Advance first attempt

```{.cpp}
template <class InputIt, class Distance>
void advance(InputIt& it, Distance n)
{
    it += n;
}
```

Does not work for linked list

## Advance second attempt

```{.cpp}
template <class InputIt, class Distance>
void advance(InputIt& it, Distance n)
{
    for (Distance i = 0; i < n; ++n) {
        ++it;
    }
}
```

Needlessly inefficient for random access iterators

## [Iterator traits](http://en.cppreference.com/w/cpp/iterator/iterator_traits)

STL has `std::iterator_traits<It>`

```{.cpp}
template <typename It>
struct iterator_traits
{
    using difference_type = ...;
    using value_type = ...;
    using pointer = ...;
    using reference = ...;
    using iterator_category = ...;
};
```

## [Iterator categories](http://en.cppreference.com/w/cpp/iterator/iterator_tags)

* Iterator categories as "tags" - empty structs
* Different types can be used for specialization!

```{.cpp}
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
```

## Advance third attempt

```{.cpp}
template <class InputIt, class Distance>
void advance(InputIt& it, Distance n) {
    using category_tag =
        typename std::iterator_traits<InputIt>::iterator_category;
    constexpr bool isRandomAccess = 
        std::is_same<category_tag, std::random_access_iterator_tag>::value;

    if (isRandomAccess) {
        it += n;
    } else {
        for (Distance i = 0; i < n; ++n) {
            ++it;
        }
    }
}
```

Still does not compile for linked list!

# Tag Dispatch

## Tag Dispatch

> Picking different code based on a tag type, using overloading

## Create functions to pick

```{.cpp}
template <class It, class Distance>
void advance_impl(std::random_access_iterator_tag, It& it, Distance n)
{
    it += n;
}

template <class It, class Distance>
void advance_impl(std::input_iterator_tag, It& it, Distance n)
{
    for (Distance i = 0; i < n; ++n) {
        ++it;
    }
}
```

## Dispatch with tags

```{.cpp}
template <class InputIt, class Distance>
void advance(InputIt& it, Distance n) {
    using category_tag =
        typename std::iterator_traits<InputIt>::iterator_category;
    advance_impl(category_tag{}, it, n);
}
```

# Variadic Templates

## Variadic Templates

> Templates that can take any number of template parameters

## Variadic Logger

Overloading + Variadic templates

```{.cpp}
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
```

## Tuple

```{.cpp}
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
};
```

# CRTP

## CRTP

* "Curiously Recurring Template Pattern"
* Allows compile-time subtype polymorphism
* No virtual functions required!

## When to CRTP?

* If you want to inherit functionality
* But know final Derived type at all times

## Comparison Operators

```{.cpp}
struct OrderEntry
{
    uint32_t price;
    uint32_t timestamp;
    
    bool operator < (OrderEntry const& other) const {
        return price < other.price
            || ((price == other.price) && timestamp < other.timestamp);
    }

    bool operator == (OrderEntry const& other) const {
        return price == other.price && timestamp == other.timestamp;
    }
};
```
    
What about `<=`, `>`, `>=`, `!=` ?

## Comparison CRTP

```{.cpp}
template <typename Derived>
struct ComparisonBase
{
private:
    Derived const& getDerived() const {
        return static_cast<Derived const&>(*this);
    }
    
public:
    bool operator >= (Derived const& other) const {
        return !(getDerived() < other);
    }

    bool operator != (Derived const& other) const {
        return !(getDerived() == other);
    }
};
```

## Comparison Operators

```{.cpp}
struct OrderEntry : ComparisonBase<OrderEntry>
{
    uint32_t price;
    uint32_t timestamp;
    
    bool operator < (OrderEntry const& other) const {
        return price < other.price
            || ((price == other.price) && timestamp < other.timestamp);
    }

    bool operator == (OrderEntry const& other) const {
        return price == other.price && timestamp == other.timestamp;
    }
};
```

# SFINAE

## SFINAE

> Substitution Failure Is Not An Error

## {data-background="../images/Wat.jpeg"}

## Template Argument Deduction

> When the compiler picks a template to instantiate, it tries to substitute types into the parameters.

```{.cpp}
template <typename T>
void printIsVector(std::vector<T> const&)
{
    std::cout << "a vector!" << std::endl;
}

template <typename T>
void printIsVector(T const&)
{
    std::cout << "not a vector!" << std::endl;
}

```

## SFINAE

> If there are several possible templates, the compiler will try all of them. If substitution fails for one, it moves onto the next without generating an error, until it succeeds. If ALL attempts fail, only then is an error generated.

This can be used for meta-programming purposes.

## Enable If

STL provides `enable_if` for SFINAE:

```{.cpp}
template <bool condition, typename T>
struct enable_if;

template <typename T=void>
struct enable_if<true, T>
{
    using type = T;
};

template <typename T=void>
struct enable_if<false, T>
{

};
```

## Tag dispatch alternative

```{.cpp}
template <class It, class Distance>
typename std::enable_if<std::is_same<
        std::random_access_iterator_tag,
        typename std::iterator_traits<It>::iterator_category
    >::value>::type
advance(It& it, Distance n)
{
    it += n;
}

template <class It, class Distance>
typename std::enable_if<!std::is_same<
        std::random_access_iterator_tag,
        typename std::iterator_traits<It>::iterator_category
    >::value>::type
advance(It& it, Distance n)
{
    for (Distance i = 0; i < n; ++n) {
        ++it;
    }
}
```

## SFINAE

* Holy hell was that ugly!
* There are other ways to use SFINAE but most of the time can be achieved with tag dispatch more cleanly
* For best usage of SFINAE see [`void_t`](http://en.cppreference.com/w/cpp/types/void_t)
