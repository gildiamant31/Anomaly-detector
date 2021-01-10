/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Gil Diamant, 314978412.
 */
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

bool SimpleAnomalyDetector::isThereWasNotCorrelation(const string &feat1, const string &feat2) {
    //scan the vector of correlated objects.
    for (auto iter = cf.begin(); iter != cf.end(); ++iter) {
        if (iter->feature1 == feat2 && iter->feature2 == feat1) {
            return false;
        }
    }
    return true;
}

correlatedFeatures
SimpleAnomalyDetector::buildNewCorr(float *x, float *y, float maxCor, int size, string fea1, string fea2) {
// fill correlatedfeatures struct.
    correlatedFeatures corrF;
    corrF.feature1 = fea1;
    corrF.feature2 = fea2;
    corrF.corrlation = maxCor;
    corrF.threshold = 0;
    //create the line regression for this struct.
    corrF.lin_reg = linear_reg(x, y, size);
    // find the maximum deviation from the line.
    for (int i = 0; i < size; ++i) {
        Point p1(x[i], y[i]);
        float dev1 = dev(p1, corrF.lin_reg);
        if ((dev1 * (float) THRESHOLD) > corrF.threshold) {
            corrF.threshold = dev1 * (float) THRESHOLD;
        }

    }
    return corrF;
    std::cout << corrF.corrlation << ", " << corrF.lin_reg.a << ", " << corrF.lin_reg.b << endl;
}

bool SimpleAnomalyDetector::isBiggerThanThreshold(float p) {
    if (p >= MIN_CORRELATION) {
        return true;
    }
    return false;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    for (auto const &it1 :ts.getDataTable()) {
        string feature1 = it1.first;
        string feature2;
        // the max correlation between two features.
        float maxCor = 0;
        int size = it1.second.size();
        for (auto const &it2 :ts.getDataTable()) {
            float xPoints[size], yPoints[size];
            if (it1 != it2) {
                for (int i = 0; i < size; ++i) {
                    xPoints[i] = it1.second[i];
                    yPoints[i] = it2.second[i];
                }
                float p = fabs(pearson(xPoints, yPoints, size));
                //if the pearson value is at least 0.9
                // and the pearson bigger then the last max correlation which checked.
                if (isBiggerThanThreshold(p) && p > maxCor) {
                    feature2 = it2.first;
                    maxCor = p;
                }
            }
        }
// if there is a value in max and also if those two features haven't
// already been saved in correlated features vector.
        if (isThereWasNotCorrelation(feature1, feature2) && maxCor > 0) {
// fill correlatedfeatures struct.
            float xPoints[size], yPoints[size];
            for (int i = 0; i < size; ++i) {
                xPoints[i] = it1.second[i];
                yPoints[i] = ts.getValues(feature2)[i];
            }
            correlatedFeatures corrF = buildNewCorr(xPoints, yPoints, maxCor, size, feature1, feature2);
            //push the object which has made to th vector.
            this->cf.push_back(corrF);
            maxCor = 0;
        }
    }
}
//check if there is deviation between the point to thr line regression
bool SimpleAnomalyDetector::isThereDev(Point p, correlatedFeatures corrF) {
    float newDev = dev(p, corrF.lin_reg);
    // this condition tell us if there is there is bigger deviation.
    if (newDev > corrF.threshold) {
        return true;
    }
    return false;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    //create anomaly report vector.
    vector<AnomalyReport> a;
    //run all over correlated features vector.
    for (correlatedFeatures corr:cf) {
        int size = ts.getValues(corr.feature1).size();
        string feature1 = corr.feature1;
        string feature2 = corr.feature2;
        for (int i = 0; i < size; ++i) {
            Point p(ts.getValues(feature1)[i], ts.getValues(feature2)[i]);
            // this condition tell us if there is there is bigger deviation.
            if (isThereDev(p, corr)) {
                //add new report of this exception.
                AnomalyReport anomaly(corr.feature1 + "-" + corr.feature2, i + 1);
                a.push_back(anomaly);
            }
        }
    }
    return a;
}

