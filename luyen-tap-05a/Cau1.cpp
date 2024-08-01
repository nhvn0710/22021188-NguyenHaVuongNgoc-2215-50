void print(Node* head)
{
	while (head != nullptr)
	{
		cout << head->value << " ";
		head = head->next;
	}
}