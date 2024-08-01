
void printReverse(Node* head, int k)
{
	if (k <= 0 || head == nullptr) return;
	if (head->next != nullptr)
	{
		printReverse(head->next, k - 1);
	}
	cout << head->value << " ";
}
