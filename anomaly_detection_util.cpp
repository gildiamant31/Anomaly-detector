/*
 * anomaly_detection_util.cpp
 *
 * Author: Gil Diamant, 314978412.
 */

#include <cmath>
#include "anomaly_detection_util.h"

float avg(float *x, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return sum / (float) size;
}

// returns the variance of X and Y
float var(float *x, int size) {
    float utl = pow(avg(x, size), 2.0);
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(x[i], 2.0);
    }
    float var = (sum / (float) size) - utl;
    return var;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    float yavg = avg(y, size);
    float xavg = avg(x, size);
    float sum = 0;
    float cov = 0;
    for (int i = 0; i < size; i++) {
        sum += (x[i] - xavg) * (y[i] - yavg);
    }
    cov = sum / (float) size;
    return cov;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    float cov1 = cov(x, y, size);
    float sig1 = sqrt(var(x, size));
    float sig2 = sqrt(var(y, size));
    return cov1 / (sig1 * sig2);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {
    float x[size];
    float y[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a, b;
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - (a * avg(x, size));
    Line line(a, b);
    return line;
}

Line linear_reg(float *x, float *y, int size) {

    float a, b;
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - (a * avg(x, size));
    Line line(a, b);
    return line;

}
// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    Line line = linear_reg(points, size);
    float dev1 = dev(p, line);
    return dev1;
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    return std::abs(p.y - l.f(p.x));
}




