
Node* findMidEl (Node* start) {

}


struct Node {
    int data;
    Node* next;


    Node (int _data, Node* next = nullptr)
        : data(_data) {}
};

int main()
{
    Node nFive(10), // last node
         nFour(5, &nFive),
         nThree(7, &nFour),
         nTwo(3, &nThree),
         start(1, &nTwo);

    findMidEl(start);    

    return 0;
}
