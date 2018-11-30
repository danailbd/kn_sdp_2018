
struct Node {
    int data;
    Node *left,
         *right;

    Node(int _data, Node* _left=nullptr, Node* _right=nullptr) {
        data = _data;
        left = _left;
        right = _right;
    }
};


int main()
{
    Node leaf_3_1(5),
         leaf_3_2(8),
         leaf_3_3(11),
         leaf_3_4(13),
         leaf_2_1(7, &leaf_3_1, &leaf_3_2),
         leaf_2_2(12, &leaf_3_3, &leaf_3_4),
         root(10, &leaf_2_1, &leaf_2_2);

    return 0;
}
