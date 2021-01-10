#include "minCircle.h"

// get circle from three points.
Circle minCircleFromThreeP(vector<Point> R) {
    //get values of those points.
    float x1 = R[0].x;
    float y1 = R[0].y;
    float x2 = R[1].x;
    float y2 = R[1].y;
    float x3 = R[2].x;
    float y3 = R[2].y;
    float A = (x1 * (y2 - y3)) - y1 * (x2 - x3) + (x2 * y3) - (x3 * y2);
    float B = ((pow(x1, 2) + pow(y1, 2)) * (y3 - y2)) + ((pow(x2, 2) + pow(y2, 2)) * (y1 - y3))
              + ((pow(x3, 2) + pow(y3, 2)) * (y2 - y1));
    float C = ((pow(x1, 2) + pow(y1, 2)) * (x2 - x3)) + ((pow(x2, 2) + pow(y2, 2)) * (x3 - x1))
              + ((pow(x3, 2) + pow(y3, 2)) * (x1 - x2));
    float D = ((pow(x1, 2) + pow(y1, 2)) * ((x3 * y2) - (x2 * y3))) +
              ((pow(x2, 2) + pow(y2, 2)) * ((x1 * y3) - (x3 * y1)))
              + ((pow(x3, 2) + pow(y3, 2)) * ((x2 * y1) - (x1 * y2)));
    // values of x and y and the radius of circle's center point.
    float xCenter = -1.0 * (B / (2 * A));
    float yCenter = -1.0 * (C / (2 * A));
    float radius = sqrt((pow(B, 2) + pow(C, 2) - (4 * A * D)) / (4 * pow(A, 2)));
    return Circle(Point(xCenter, yCenter), radius);
}


//get the middle point between two points.
Point getMidPoint(Point p1, Point p2) {
    float x = (p1.x + p2.x) / 2;
    float y = (p1.y + p2.y) / 2;
    Point center(x, y);
    return center;
}

//calculate the distance between two points.
float distance(Point p1, Point p2) {
    float x = pow(p1.x - p2.x, 2);
    float y = pow(p1.y - p2.y, 2);
    return sqrt(x + y);
}

// get circle from only two points.
Circle getCircleFromTwo(Point p1, Point p2) {
    Point center = getMidPoint(p1, p2);
    float radius = distance(p1, p2) / 2;
    return Circle(center, radius);
}

//check if the point is inside the circle
//by measure the distance between the point to circle's center point.
bool isInCircle(Point p, Circle c) {
    if (distance(c.center, p) > c.radius) {
        return false;
    }
    return true;
}

Circle recursiveFindMinCircle(Point **points, size_t size, vector<Point> R) {
    if (R.size() == 3) {
        return minCircleFromThreeP(R);
    }
    if (size == 0) {
        switch (R.size()) {
            //case 0 must return circle, because in the recursion we will always get here.
            //can be handeld with "try/catch" but much more complicated
            case 0:
                return Circle(Point(0, 0), 0);
            case 1:
                return Circle(R[0], 0);
            case 2:
                return getCircleFromTwo(R[0], R[1]);
                //case 3 already handled with the previous if
            default:
                throw runtime_error("error");
        }
    }
    // redundant
    //vector<Point> correntR (R);
    Circle circle = recursiveFindMinCircle(points, (size - 1), R);
    if (isInCircle(*points[size - 1], circle)) {
        return circle;
    }
    R.push_back(*points[size - 1]);
    return recursiveFindMinCircle(points, (size - 1), R);


}

// implement
Circle findMinCircle(Point **points, size_t size) {
    return recursiveFindMinCircle(points, size, {});
}