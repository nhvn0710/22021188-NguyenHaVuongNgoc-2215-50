int* getPointerToArray(int n)
{
    int* A = new int[n];
    for (int i = 0; i < n; i++)
    {
        cin >> A[i];
    }
    return A;
}