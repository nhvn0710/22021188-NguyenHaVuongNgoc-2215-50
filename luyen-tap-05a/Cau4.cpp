Node* deleteNode(Node* head, int pos)
{
    if (pos == 0)
    {
        Node* tmp = head->next;
        delete head;
        return tmp;
    }
    Node* p = head;
    while (pos > 1 && p->next != nullptr)
    {
        p = p->next;
        pos--;
    }
    Node* tmp = p->next;
    p->next = p->next->next;
    delete tmp;
    return head;
}