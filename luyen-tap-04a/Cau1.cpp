struct Point
{
    double x, y;
    Point();
    Point(double, double);
    Point(const Point&);
};

class Triangle
{
    Point p1, p2, p3;
public:
    Triangle(const Point&, const Point&, const Point&);
    double getPerimeter() const;
    double getArea() const;
};



Point::Point()
{
    x = 0;
    y = 0;
}
Point::Point(double _x, double _y)
{
    x = _x;
    y = _y;
}
Point::Point(const Point& _p)
{
    x = _p.x;
    y = _p.y;
}
Triangle::Triangle(const Point& _p1, const Point& _p2, const Point& _p3)
{
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;
}
double distance(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
double Triangle::getPerimeter() const
{
    return distance(p1, p2) + distance(p2, p3) + distance(p3, p1);
}
double Triangle::getArea() const
{
    double h = (this->getPerimeter()) / 2;
    return sqrt(h * (h - distance(p1, p2)) * (h - distance(p3, p2)) * (h - distance(p1, p3)));
}
