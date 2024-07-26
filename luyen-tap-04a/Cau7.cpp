void rFilter(char* s) {
	int len = strlen(s);
	if (len == 0) return;

	char* right = s + len - 1;
	while (right >= s && !((*right >= 'a' && *right <= 'z') || (*right >= 'A' && *right <= 'Z'))) {
		*right = '_';
		right--;
	}
}