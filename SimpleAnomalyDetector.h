
/*
 * SimpleAnomalyDetector.h
 *
 * Author: Gil Diamant, 314978412.
 */
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"

#define MIN_CORRELATION 0.9
#define THRESHOLD 1.2

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float radius;
    Point center;
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> cf;
public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);

    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }

    // return false if there was correlation before, if wasn't return true.
    bool isThereWasNotCorrelation(const string &feat1, const string &feat2);

    virtual correlatedFeatures buildNewCorr(float *x, float *y, float maxCor, int size, string fea1, string fea2);

    virtual bool isBiggerThanThreshold(float p);

    virtual bool isThereDev(Point p,correlatedFeatures corrF);

};


#endif /* SIMPLEANOMALYDETECTOR_H_ */
