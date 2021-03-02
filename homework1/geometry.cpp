#include "geometry.h"
#include <iostream>
#include <cassert>
#include <math.h>

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
	//todo differents sizes?..
	for (int i = 0; i < N_; i++)
		points_[i] = other.points_[i];
	N_ = other.N_;

	return *this;
}
double PolygonalChain:: perimeter() const {
	//todo no capital letters for vars
	double P = 0;
	for (int i = 0; i < getN()-1; i++)
		P += sqrt(pow(getPoint(i).getX()- getPoint(i+1).getX(), 2)+ pow(getPoint(i).getY() - getPoint(i + 1).getY(), 2));
	return P;
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
	//todo use perimeter from base class
	double P = 0;
	for (int i = 0; i < getN() - 1; i++)
		P += sqrt(pow(getPoint(i).getX() - getPoint(i + 1).getX(), 2) + pow(getPoint(i).getY() - getPoint(i + 1).getY(), 2));
	P += sqrt(pow(getPoint(getN() - 1).getX() - getPoint(0).getX(), 2) + pow(getPoint(getN() - 1).getY() - getPoint(0).getY(), 2));
	return P;
}

//todo constructor from base class
Polygon::Polygon(int N, Point* points) {
	N_ = N;
	points_ = new Point[N];
	for (int i = 0; i < N; i++)
		points_[i] = points[i];
}
Polygon::Polygon(const Polygon& other) {
	N_ = other.N_;
	points_ = new Point[N_];
	for (int i = 0; i < N_; i++)
		points_[i] = other.points_[i];
}
//todo = from base class
Polygon& Polygon:: operator=(const Polygon& other) {
	if (&other == this)
		return *this;

	for (int i = 0; i < N_; i++)
		points_[i] = other.points_[i];
	N_ = other.N_;

	return *this;
}
double Polygon::perimeter() const {
	//todo perimeter from base class, u dont need it here
	double P = 0;
	for (int i = 0; i < N_ - 1; i++)
		P += sqrt(pow(getPoint(i).getX() - getPoint(i + 1).getX(), 2) + pow(getPoint(i).getY() - getPoint(i + 1).getY(), 2));
	P += sqrt(pow(getPoint(N_ - 1).getX() - getPoint(0).getX(), 2) + pow(getPoint(N_ - 1).getY() - getPoint(0).getY(), 2));
	return P;
}
double Polygon::area() const {
	int S = 0;
	for (int i = 0; i < N_ - 1; i++) {
		S += getPoint(i).getX() * getPoint(i + 1).getY() - getPoint(i + 1).getX() * getPoint(i).getY();
	}
	//todo weird
	S = 0.5 * (abs(S + (getPoint(N_ - 1).getX() * getPoint(0).getY()) - (getPoint(0).getX() * getPoint(N_ - 1).getY())));
	double A = (double)S;
	return A;
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
//todo area and perimeter for regular polygon
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

