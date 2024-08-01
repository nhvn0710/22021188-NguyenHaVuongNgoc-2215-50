void reverse(char* s)
{
    int len = strlen(s);
    if (len < 2) return;
    char* l = s;
    char* r = s + len - 1;
    while (l < r)
    {
        char c = *l;
        *l = *r;
        *r = c;
        l++;
        r--;
    }
}