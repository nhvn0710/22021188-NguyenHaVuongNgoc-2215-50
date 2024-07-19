struct Time {
    int h;
    int m;
    int s;

    Time()
    {
        h = 0; m = 0; s = 0;
    }
    Time(int h0, int m0, int s0)
    {
        h = h0; m = m0; s = s0;
    }

    int second() {
        return h * 3600 + m * 60 + s;
    }

    void print() {
        if (h < 10) cout << "0";
        cout << h << ":";
        if (m < 10) cout << "0";
        cout << m << ":";
        if (s < 10) cout << "0";
        cout << s << endl;
    }
};

Time normalize(int h, int m, int s) {
    if (s >= 60)
    {
        m = m + s / 60;
        s = s % 60;
    }
    if (m >= 60)
    {
        h = h + m / 60;
        m = m % 60;
    }
    return { h, m, s };
}