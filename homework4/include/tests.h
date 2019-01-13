#pragma once

#include <iostream>
#include <list>
#include <string>

using std::cerr;
using std::cout;
using std::endl;


template <class T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& l) {
    os << "[ ";
    for (auto i : l) {
        os << i << " ";
    }
    os << "]";
    return os;
}

class UnitTests {
    public:
        UnitTests(std::string _testCaseName) {
            testCaseName = _testCaseName;
            tests_counter = 0;
            failed_tests_counter = 0;
        };

        template <typename T>
        void AssertEq(T expected, T value)
        {
            tests_counter++;
            if (expected != value) {
                cerr << "FAIL(" << tests_counter << "): Values mismatch!"
                    << "\n-- expected: " << expected << "\n---- actual: " << value << endl;
                failed_tests_counter++;
            } else {
                cout << "." << endl;
            }
        }

        void PrintTestsResult() {
            cout << "Test case - " << testCaseName << " - ";
            if (failed_tests_counter)
                cout << "Failure: ";
            else
                cout << "Success: ";
            cout << tests_counter-failed_tests_counter << "|" << tests_counter << " tests." << endl;
        }
    private:
        std::string testCaseName;
        int tests_counter;
        int failed_tests_counter;
};


