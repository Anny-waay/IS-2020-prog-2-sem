#pragma once
class Point {
private:
	int x_;
	int y_;
public:
	Point(int x = 0, int y = 0);
	Point(const Point& other);
	Point& operator=(const Point& other);
	int getX() const { return x_;}
	int getY() const { return y_;}
};

class PolygonalChain {
private:
	Point* points_;
	int N_;
public:
	PolygonalChain(int N, Point* points);
	PolygonalChain(const PolygonalChain& other);
	PolygonalChain& operator=(const PolygonalChain& other);
	int getN() const { return N_; }
	Point getPoint(int num) const { return  points_[num]; }
	virtual double perimeter() const;
};

class ClosedPolygonalChain : public PolygonalChain {
public:
	ClosedPolygonalChain(int N, Point* points);
	ClosedPolygonalChain(const ClosedPolygonalChain& other);
	ClosedPolygonalChain& operator=(const ClosedPolygonalChain& other);
	double perimeter() const override;
};

class Polygon {
private:
	Point * points_;
	int N_;
public:
	Polygon(int N, Point* points);
	Polygon(const Polygon& other);
	Polygon& operator=(const Polygon& other);
	int getN() const { return N_; }
	Point getPoint(int num) const { return  points_[num]; }
	double perimeter() const;
	double area() const;
};

class Triangle : public Polygon {
public:
	Triangle(int N, Point* points);
	Triangle(const Triangle& other);
	Triangle& operator=(const Triangle& other);
	int hasRightAngle() const;
};

class Trapezoid : public Polygon {
public:
	Trapezoid(int N, Point* points);
	Trapezoid(const Trapezoid& other);
	Trapezoid& operator=(const Trapezoid& other);
	double height() const;
};

class RegularPolygon : public Polygon {
public:
	RegularPolygon(int N, Point* points);
	RegularPolygon(const RegularPolygon& other);
	RegularPolygon& operator=(const RegularPolygon& other);
};