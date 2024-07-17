bool is_prime(int n)
{
    if (n < 2) { return false; }
    if (n < 4) { return true; }
    if (n % 2 == 0) return false;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

void print_prime(int n)
{
    for (int i = 1; i < n; i++)
    {
        if (is_prime(i))
        {
            cout << i << endl;
        }
    }
}