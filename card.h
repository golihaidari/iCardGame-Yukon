typedef struct Node {
    int row;
    int column;
    char suit;
    char value;
    char* face;
    char* color;
    struct Node* next;
} Card;



