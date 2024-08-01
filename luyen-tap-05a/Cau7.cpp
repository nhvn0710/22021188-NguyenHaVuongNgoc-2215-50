Node* deleteSingle(Node* head) {
    if (!head || !head->next || !head->next->next) {
        return head;
    }

    Node* p1 = head;
    Node* p2 = head->next;

    if (p1->value != p2->value && p2->value == p2->next->value) {
        head = p2;
        delete p1;
        return head;
    }

    while (p2->next != nullptr) {
        if (p1->value != p2->value && p2->value != p2->next->value) {
            p1->next = p2->next;
            delete p2;
            return head;
        }
        p1 = p2;
        p2 = p2->next;
    }

    if (p1->value != p2->value) {
        p1->next = nullptr;
        delete p2;
    }

    return head;
}