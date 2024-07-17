int* getPointerToArray(int n) {
    int* arr;
    arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        int tmp;
        cin >> tmp;
        arr[i] = tmp;
    }
    return arr;
}