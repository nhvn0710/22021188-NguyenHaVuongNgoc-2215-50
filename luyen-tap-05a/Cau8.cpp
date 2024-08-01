Node* deleteDuplicates(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Node* p = head;
    while (p != nullptr && p->next != nullptr) {
        Node* rp = p;
        while (rp->next != nullptr) {
            if (rp->next->value == p->value) {
                Node* p0 = rp->next;
                rp->next = rp->next->next;
                delete p0;
            }
            else {
                rp = rp->next;
            }
        }
        p = p->next;
    }

    return head;
}