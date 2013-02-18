#include <stdafx.h>
#include "graphics.h"

using namespace std;

// область імен дозволяє уникнути
// конфліктів в іменах класів з іншими бібліотеками
namespace graphics 
{

Point::Point() { 
	x = 0; 
	y = 0; 
}

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}	

Point::Point(const Point& p) {
	x = p.x;
	y = p.y;
}

double Point::dist(const Point &p) const {
	double dx = x - p.x;
	double dy = y - p.y;
	return sqrt(dx*dx + dy*dy);
}

int Point::operator == (const Point& that) {
	return x == that.x && y == that.y;
}

Figure::~Figure(){}

AbstractAngledFigure::AbstractAngledFigure(int pointsCount) {
	if(pointsCount <= 0) {
		throw "Illegal points count";
	}

	this->points = new Point[pointsCount];
	this->pointsCount = pointsCount;
	this->color = RGB(0,0,0);
}

AbstractAngledFigure::~AbstractAngledFigure() {
	if(points) {
		delete points;
		points = NULL;
	}
}

Point& AbstractAngledFigure::getPoint(int index) {
	return points[index];
}

Point& AbstractAngledFigure::getPrevPoint(int index) {
	return index == 0 ? points[pointsCount-1] : points[index - 1];
}

Point& AbstractAngledFigure::getNextPoint(int index) {
	return index == pointsCount-1 ? points[0] : points[index + 1];
}

int AbstractAngledFigure::getPointsCount() {
	return pointsCount;
}

void AbstractAngledFigure::setColor(COLORREF color) {
	this->color = color;
}

COLORREF AbstractAngledFigure::getColor() {
	return this->color;
}

double AbstractAngledFigure::perimeter() {
	// вироджений випадок для відрізка
	if(pointsCount == 2) {
		return getPoint(0).dist(getNextPoint(0)); 	
	}

	double per = 0;
	for(int i = 0; i < pointsCount; i++) {
		per += getPoint(i).dist(getNextPoint(i));
	}

	return per;
}

void AbstractAngledFigure::draw(GraphicsBridge& g) { 
	// малювання кожної сторони багатокутника
	for(int i = 0; i < pointsCount; i++) {
		Segment s = getSide(i);

		Point sp = s.getPoint(0);
		Point ep = s.getPoint(1);
		
		g.line(sp.getX(), sp.getY(), ep.getX(), ep.getY(), color);
	}
}

Segment AbstractAngledFigure::getSide(int i) {
	return Segment(points[i], getNextPoint(i));
}

Segment::Segment(const Point& p1, const Point& p2) : AbstractAngledFigure(2) {
	points[0] = p1;
	points[1] = p2;
}

double Segment::getA() {
	return points[0].getY() - points[1].getY();
}

double Segment::getB() {
	return points[1].getX() - points[0].getX();
}

double Segment::getC() {
	return (double)points[0].getX()*points[1].getY() - 
		(double)points[1].getX()*points[0].getY();
}

double Segment::getRelLocation(Point& p) {
	return getA()*p.getX() + getB()*p.getY() + getC();
}

bool Segment::intersects(Segment& s) {
	// Перевіряється положення кінців переданого відрізка
	// відносно поточного і навпаки 
	double sp0 = getRelLocation(s.getPoint(0));
	double sp1 = getRelLocation(s.getPoint(1));

	double p0 = s.getRelLocation(getPoint(0));
	double p1 = s.getRelLocation(getPoint(1));

	return (sign(sp0) != sign(sp1) && 
			sign(p0) != sign(p1));
}

double Segment::angle(Segment& s) {
	// angle between two straights equals angle beetwen
	// direction vectors (-B1;A1) and (-B2;A2)

	double A1 = getA();
	double A2 = s.getA();
	double B1 = getB();
	double B2 = s.getB();

	double num = B1*B2 + A1*A2;
	double denum = sqrt(B1*B1 + A1*A1) * sqrt(B2*B2 + A2*A2);

	double val = num/denum;
	
	// precision fix - sometimes value becаme slightly > 1 (e.g. 1.00000001)
	if(val > 1 || val < 1) val = floor(val + 0.5);

	return acos(val);
}

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3) : AbstractAngledFigure(3) {
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
}

Quadrangle::Quadrangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4) : AbstractAngledFigure(4) {
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
}

} // end of namespace