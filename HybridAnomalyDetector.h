

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
    float generalThreshold = 0.9;
public:
    HybridAnomalyDetector() {}


    virtual ~HybridAnomalyDetector();

    correlatedFeatures buildNewCorr(float *x, float *y, float maxCor, int size, string fea1, string fea2);

    bool isBiggerThanThreshold(float p);

    bool isThereDev(Point p, correlatedFeatures corrF);

    void addCorrByCircle(string feature1, string feature2, float cor, TimeSeries ts, int size);

    float getThreshold() {
        return this->generalThreshold;
    }

    void setNewThreshold(float newValue) {
        this->generalThreshold = newValue;
    }
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
