Node* insert(Node* head, int value)
{
    Node* np = new Node;
    np->value = value;
    np->next = nullptr;
    if (head == nullptr)
    {
        head = np;
        return head;
    }
    Node* p = head;
    if (p->next == nullptr)
    {
        if (p->value < value)
        {
            p->next = np;
        }
        else
        {
            np->next = p;
            head = np;
        }
        return head;
    }
    if (p->value < p->next->value) {
        if (value < p->value)
        {
            np->next = p;
            head = np;
            return head;
        }
        while (p->next != nullptr)
        {
            if (p->next->value > value)
            {
                np->next = p->next;
                p->next = np;
                return head;
            }
            else
            {
                p = p->next;
            }
        }
    }
    else
    {
        if (value > p->value)
        {
            np->next = p;
            head = np;
            return head;
        }
        while (p->next != nullptr)
        {
            if (p->next->value < value)
            {
                np->next = p->next;
                p->next = np;
                return head;
            }
            else
            {
                p = p->next;
            }
        }
    }
    p->next = np;
    return head;
}