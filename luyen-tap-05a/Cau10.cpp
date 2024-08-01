Node* sort(Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    Node* outp = nullptr;
    Node* p = head;

    while (p != nullptr) {
        Node* next = p->next;

        if (outp == nullptr || outp->getValue() >= p->getValue()) {
            p->next = outp;
            p->prev = nullptr;
            if (outp != nullptr) {
                outp->prev = p;
            }
            outp = p;
        }
        else {
            Node* sp = outp;
            while (sp->next != nullptr && sp->next->getValue() < p->getValue()) {
                sp = sp->next;
            }
            p->next = sp->next;
            if (sp->next != nullptr) {
                sp->next->prev = p;
            }
            sp->next = p;
            p->prev = sp;
        }
        p = next;
    }

    return outp;
}