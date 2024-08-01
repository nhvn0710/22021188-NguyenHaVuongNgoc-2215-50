Node* insertHead(Node* head, int value)
{
    Node* p = new Node;
    p->value = value;
    p->next = head;
    return p;
}