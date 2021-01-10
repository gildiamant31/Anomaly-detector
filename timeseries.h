/*
 * timeseries.h
 *
 * Author: Gil Diamant, 314978412.
 */
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries {
    //our keys on the map will be the features names.
    map<string, vector<float>> data_table;
    //vector of features names.
    vector<string> feature_name;
public:
    TimeSeries(const char *CSVfileName);

    void readCSVFile(const char *CSVfileName);

    //get all the data from the map.
    map<string, vector<float>> getDataTable() const;

    //get a specific values vector according to the feature name.
    vector<float> getValues(string str) const;

    // this function will help us to get vector of words without punctuation marks.
    vector<string> dataSeparation(string line, char comma);

    //add values to the map.
    void addValuesToTHeMap(string line, vector<string> values);

};


#endif /* TIMESERIES_H_ */
