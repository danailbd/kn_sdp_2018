#include "include/tests.h"


template <class K, class V>
class HashMap
{
public:
    HashMap (int capacity) {};

    void add (K key, V value) {};
    void remove (K key) {};
    V find (K key, V value) {};

    int size() const { return 0; };

private:

};


void hashMap_basic_test() {
    UnitTests suit("Basic tests");
    
    HashMap<int, char*> testHashMap(10);

    char value[50] = "some";
    testHashMap.add(50, value);
    testHashMap.add(10, value);


    suit.AssertEq(2, testHashMap.size());

    suit.PrintTestsResult();
}

int main()
{
    hashMap_basic_test();

    return 0;
}
