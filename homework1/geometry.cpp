#include "geometry.h"
#include <iostream>
#include <cassert>
#include <math.h>
#define PI 3.1415926535

Point::Point(int x, int y)
	: x_(x)
	, y_(y)
{}
Point::Point(const Point& other)
	: x_(other.x_)
	, y_(other.y_)
{}

Point& Point:: operator= (const Point& other) {
	if (&other == this)
		return *this;

	x_ = other.x_;
	y_ = other.y_;

	return *this;
}

PolygonalChain:: PolygonalChain(int N, Point* points){
	N_ = N;
	points_ = new Point[N];
	for (int i = 0; i < N; i++) {
		points_[i] = points[i];
	}
}
PolygonalChain:: PolygonalChain(const PolygonalChain& other){
	N_ = other.N_;
	points_ = new Point[N_];
	for (int i = 0; i < N_; i++)
		points_[i] = other.points_[i];
}
PolygonalChain& PolygonalChain:: operator=(const PolygonalChain& other) {
	if (&other == this)
		return *this;
	//fixed differents sizes?..
	delete[]points_;
	N_ = other.N_;
	points_ = new Point[N_];
	for (int i = 0; i < N_; i++)
		points_[i] = other.points_[i];

	return *this;
}
PolygonalChain:: ~PolygonalChain() {
	delete[] points_;
	points_ = nullptr;
}
double PolygonalChain:: perimeter() const {
	//fixed no capital letters for vars
	double per = 0;
	for (int i = 0; i < getN()-1; i++)
		per += sqrt(pow(getPoint(i).getX()- getPoint(i+1).getX(), 2)+ pow(getPoint(i).getY() - getPoint(i + 1).getY(), 2));
	return per;
}

ClosedPolygonalChain::ClosedPolygonalChain(int N, Point* points)
	:PolygonalChain(N, points)
{}
ClosedPolygonalChain::ClosedPolygonalChain(const ClosedPolygonalChain& other)
	:PolygonalChain(other)
{}
ClosedPolygonalChain& ClosedPolygonalChain:: operator=(const ClosedPolygonalChain& other) {
	PolygonalChain:: operator=(other);
	return *this;
}
double ClosedPolygonalChain::perimeter() const {
	//fixed use perimeter from base class
	double per = PolygonalChain:: perimeter();
	per += sqrt(pow(getPoint(getN() - 1).getX() - getPoint(0).getX(), 2) + pow(getPoint(getN() - 1).getY() - getPoint(0).getY(), 2));
	return per;
}

//fixed constructor from base class
Polygon::Polygon(int N, Point* points) 
	:ClosedPolygonalChain(N, points)
{}
Polygon::Polygon(const Polygon& other) 
	:ClosedPolygonalChain(other)
{}
//fixed = from base class
Polygon& Polygon:: operator=(const Polygon& other) {
	ClosedPolygonalChain:: operator=(other);
	return *this;
}
//fixed perimeter from base class, u dont need it here
double Polygon::area() const {
	int sum = 0;
	for (int i = 0; i < getN() - 1; i++) {
		sum += getPoint(i).getX() * getPoint(i + 1).getY() - getPoint(i + 1).getX() * getPoint(i).getY();
	}
	//fixed weird
	sum += getPoint(getN() - 1).getX() * getPoint(0).getY() - getPoint(0).getX() * getPoint(getN() - 1).getY();
	return 0.5 * abs(sum);
}

Triangle::Triangle(int N, Point* points) 
	: Polygon(N, points)
{}
Triangle::Triangle(const Triangle& other) 
	:Polygon(other)
{}
Triangle& Triangle:: operator=(const Triangle& other) {
	Polygon:: operator=(other);
	return *this;
}

int Triangle::hasRightAngle() const{
	int v1x = getPoint(1).getX() - getPoint(0).getX();
	int v1y = getPoint(1).getY() - getPoint(0).getY();
	int v2x = getPoint(2).getX() - getPoint(1).getX();
	int v2y = getPoint(2).getY() - getPoint(1).getY();
	int v3x = getPoint(2).getX() - getPoint(0).getX();
	int v3y = getPoint(2).getY() - getPoint(0).getY();
	if (v1x * v2x + v1y * v2y == 0)
		return 1;
	if (v3x * v2x + v3y * v2y == 0)
		return 1;
	if (v1x * v3x + v1y * v3y == 0)
		return 1;
	return 0;
}

Trapezoid::Trapezoid(int N, Point* points) 
	: Polygon(N, points)
{}
Trapezoid::Trapezoid(const Trapezoid& other) 
	: Polygon(other)
{}
Trapezoid& Trapezoid:: operator=(const Trapezoid& other) {
	Polygon:: operator=(other);
	return *this;
}
double Trapezoid::height() const {
	double H = (2 * area()) / ((sqrt(pow(getPoint(1).getX() - getPoint(2).getX(), 2) + pow(getPoint(1).getY() - getPoint(2).getY(), 2))) + (sqrt(pow(getPoint(3).getX() - getPoint(0).getX(), 2) + pow(getPoint(3).getY() - getPoint(0).getY(), 2))));
	return H;
}

RegularPolygon::RegularPolygon(int N, Point* points)
	: Polygon(N, points)
{}
RegularPolygon::RegularPolygon(const RegularPolygon& other)
	: Polygon(other)
{}
RegularPolygon& RegularPolygon:: operator=(const RegularPolygon& other) {
	Polygon:: operator=(other);
	return *this;
}
//fixed area and perimeter for regular polygon
double RegularPolygon::perimeter() const {
	double side = sqrt(pow(getPoint(0).getX() - getPoint(1).getX(), 2) + pow(getPoint(0).getY() - getPoint(1).getY(), 2));
	return getN() * side;
}
double RegularPolygon::area() const {
	int side = pow(getPoint(0).getX() - getPoint(1).getX(), 2) + pow(getPoint(0).getY() - getPoint(1).getY(), 2);
	int numerator = getN() * side;
	double area = numerator / (4 * tan(PI / getN()));
	return area;
}
