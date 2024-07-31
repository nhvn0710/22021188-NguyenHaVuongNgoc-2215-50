void rFilter(char* s) {
    int len = strlen(s);
    if (len == 0) return;
    char* r = s + len - 1;
    while (r >= s && !((*r >= 'a' && *r <= 'z') || (*r >= 'A' && *r <= 'Z'))) {
        *r = '_';
        r--;
    }
}