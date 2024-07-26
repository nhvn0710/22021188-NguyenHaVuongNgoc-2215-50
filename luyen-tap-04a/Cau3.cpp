struct Complex {
    int a;
    int b;
    Complex()
    {
        a = 0;
        b = 0;
    }
    Complex(int _a, int _b)
    {
        a = _a;
        b = _b;
    }

    double abs() {
        return sqrt(a * a + b * b);
    }

    void print() {
        if (b == 0)
        {
            cout << a;
        }
        else
        {
            if (a != 0) cout << a;
            if (b > 0) cout << "+";
            if (b == -1) cout << "-";
            if (b != 1 && b != -1) cout << b;
            cout << "i";
        }
        cout << endl;
    }
};

Complex add(Complex a, Complex b) {
    Complex c(a.a + b.a, a.b + b.b);
    return c;
}