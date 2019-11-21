
Node* findMidEl (Node* start) {

}


struct Node {
    int data;
    Node* next;


    Node (int _data, Node* _next = nullptr)
        : data(_data), next(_next) {}
};

int main()
{
    Node nFive(10); // last node
    Node nFour(5, &nFive);
    Node nThree(7, &nFour);
    Node nTwo(3, &nThree);
    Node start(1, &nTwo); // first node

    findMidEl(start);    

    return 0;
}
