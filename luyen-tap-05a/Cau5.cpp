void printLast(Node* head, int k)
{
    int len = 0;
    Node* p = head;
    while (p != nullptr)
    {
        len++;
        p = p->next;
    }
    p = head;
    while (p != nullptr)
    {
        k++;
        if (k > len)
        {
            cout << p->value << " ";
        }
        p = p->next;
    }
}