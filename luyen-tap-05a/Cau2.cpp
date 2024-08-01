Node* insertTail(Node* head, int value)
{
    if (head == nullptr)
    {
        Node* p = new Node;
        p->value = value;
        p->next = nullptr;
        return p;
    }
    Node* p = head;
    while (p->next != nullptr)
    {
        p = p->next;
    }
    Node* tmp = new Node;
    tmp->value = value;
    tmp->next = nullptr;
    p->next = tmp;
    return head;
}