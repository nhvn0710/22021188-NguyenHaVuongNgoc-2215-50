struct Rectangle {

    int height;
    int length;

    Rectangle() {
        height = 0;
        length = 0;
    }
    Rectangle(int a, int b) {
        height = a;
        length = b;
    }

    int getPerimeter() {
        return 2 * (height + length);
    }

    void print() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < length; j++) {
                if (i == 0 || i == height - 1 || j == 0 || j == length - 1) {
                    cout << '*';
                }
                else {
                    cout << ' ';
                }
            }
            cout << endl;
        }
    }
};

int compare(Rectangle a, Rectangle b) {
    int pA = a.getPerimeter();
    int pB = b.getPerimeter();

    if (pA < pB) {
        return -1;
    }
    else if (pA == pB) {
        return 0;
    }
    else {
        return 1;
    }
}