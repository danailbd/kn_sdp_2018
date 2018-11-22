
#include<iostream>
#include <unistd.h>
#include<array>

#include<fstream>



using namespace std;


void insertionSort(int* arr, int size) {
    int tmp;
    for (int i = 1; i < size-1; ++i) {
        for (int j = i+1; j >= 0; --j) {
            if (arr[j-1] > arr[j]) {
                swap(arr[j], arr[j-1]);
            }
        }
    }
}

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT> 
                            (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};


void printArray (int* arr, int size) {
    for (int i = 0; i < size-1; ++i) {
        cout << arr[i] << ", ";
    }
    cout << arr[size-1];
}


void measureInsertionSort()
{
    const int SIZE = 100000;
    /* const int SIZE = 100000;  very slow */

    int arr[SIZE];

    for (int i = 0; i < SIZE; i++) {
        arr[i] = SIZE-i;
    }
    std::cout << "Reversed order: " << measure<std::chrono::milliseconds>::execution(insertionSort, arr, SIZE) << std::endl;

    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
    std::cout << "Sorted: " << measure<std::chrono::milliseconds>::execution(insertionSort, arr, SIZE) << std::endl;
}


int main()
{
    measureInsertionSort();

    return 0;
}
