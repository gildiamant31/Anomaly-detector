
#include "HybridAnomalyDetector.h"


HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

//making a distinction between the Hybrid detector to the Simple.
bool HybridAnomalyDetector::isBiggerThanThreshold(float p) {
    if (p < MIN_CORRELATION && p > 0.5) {
        return true;
    } else {
        return SimpleAnomalyDetector::isBiggerThanThreshold(p);
    }
}

//check if this point is inside the circle and return fals if it isn't.
bool HybridAnomalyDetector::isThereDev(Point p, correlatedFeatures corrF) {
    if (corrF.corrlation < MIN_CORRELATION && corrF.corrlation > 0.5) {
        //build circle from center point and the threshold(radius*1.1).
        Circle c(corrF.center, corrF.threshold);
        if (!isInCircle(p, (p, c))) {
            //there is deviation!
            return true;
        }
        return false;
    }
    return SimpleAnomalyDetector::isThereDev(p, corrF);
}

correlatedFeatures HybridAnomalyDetector::buildNewCorr(float *x, float *y, float maxCor, int size, string fea1,
                                                       string fea2) {
    //if this condition occur wh have to create corrF with the minCircle class.
    if (maxCor < MIN_CORRELATION && maxCor > 0.5) {
        // fill correlatedfeatures struct.
        correlatedFeatures corrF;
        corrF.feature1 = fea1;
        corrF.feature2 = fea2;
        corrF.corrlation = maxCor;
        corrF.threshold = 0;
        Point *pts[size];
        // find the maximum deviation from the line.
        for (int i = 0; i < size; ++i) {
            pts[i] = new Point(x[i], y[i]);
        }
        corrF.radius = findMinCircle(pts, size).radius;
        corrF.center = findMinCircle(pts, size).center;
        corrF.threshold = corrF.radius * (THRESHOLD - 0.1);
        for (int i = 0; i < size; ++i) {
            delete pts[i];
        }
        return corrF;
    } else {
        //if i >= the min corrlation which had set.
        return SimpleAnomalyDetector::buildNewCorr(x, y, maxCor, size, fea1, fea2);
    }
}