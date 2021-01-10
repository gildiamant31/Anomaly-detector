
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include "math.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

float distance(Point p1, Point p2);

Circle getCircleFromTwo(Point p1, Point p2);

Point getMidPoint(Point p1, Point p2);

Circle minCircleFromThreeP(vector<Point> R);

Circle recursiveFindMinCircle(Point **points, size_t size, vector<Point> R);

bool isInCircle(Point p, Circle c);


Circle findMinCircle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
