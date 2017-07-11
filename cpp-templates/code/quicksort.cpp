#include <iostream>
#include <vector>
#include <list>

int* partitionA(int* begin, int* end)
{
    int pivot = *begin;
    int* pivotDestination = begin;
    int* cursor = begin;

    while (++cursor != end)
    {
        if (*cursor <= pivot)
        {
            std::swap(*(++pivotDestination), *cursor);
        }
    }
    std::swap(*pivotDestination, *begin);
    return pivotDestination;
}

void quicksortA(int* begin, int* end)
{
    auto numElements = end - begin;
    if (numElements < 2) { return; }

    int* pivot = partitionA(begin, end);
    quicksortA(begin, pivot);
    quicksortA(++pivot, end);
}


// ---------------------------------------------
template <typename T>
T* partitionB(T* begin, T* end)
{
    T pivot = *begin;
    T* pivotDestination = begin;
    T* cursor = begin;

    while (++cursor != end)
    {
        if (*cursor <= pivot)
        {
            std::swap(*(++pivotDestination), *cursor);
        }
    }
    std::swap(*pivotDestination, *begin);
    return pivotDestination;
}

template <typename T>
void quicksortB(T* begin, T* end)
{
    auto numElements = end - begin;
    if (numElements < 2) { return; }

    T* pivot = partitionB(begin, end);
    quicksortB(begin, pivot);
    quicksortB(++pivot, end);
}

// ---------------------------------------------
template <typename It>
It partitionC(It begin, It end)
{
    auto pivot = *begin;
    It pivotDestination = begin;
    It cursor = begin;

    while (++cursor != end)
    {
        if (*cursor <= pivot)
        {
            std::swap(*(++pivotDestination), *cursor);
        }
    }
    std::swap(*pivotDestination, *begin);
    return pivotDestination;
}

template <typename It>
void quicksortC(It begin, It end)
{
    auto numElements = std::distance(begin, end);
    if (numElements < 2) { return; }

    It pivot = partitionC(begin, end);
    quicksortC(begin, pivot);
    quicksortC(++pivot, end);
}

int main(int argc, char* argv[])
{
    // std::vector<int> values = {23,5,567,8,823,12,2,5,6,7,989,2,1223,5,71,1};
    std::list<int> values = {23,5,567,8,823,12,2,5,6,7,989,2,1223,5,71,1};

    quicksortC(std::begin(values), std::end(values));

    for (auto elem : values)
    {
        std::cout << elem << std::endl;
    }

    return 0;
}
